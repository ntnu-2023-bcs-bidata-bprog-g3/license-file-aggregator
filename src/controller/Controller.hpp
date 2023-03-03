#ifndef Controller_hpp
#define Controller_hpp

#include "dto/DTOs.hpp"

#include "shared.hpp"
#include "file/fileHandler.hpp"
#include "ssl/certificates.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/data/stream/FileStream.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/mime/multipart/TemporaryFileProvider.hpp"
#include "oatpp/web/mime/multipart/Reader.hpp"
#include "oatpp/web/mime/multipart/PartList.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <string>
#include <map>

namespace multipart = oatpp::web::mime::multipart;

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class Controller : public oatpp::web::server::api::ApiController {
public:
	/**
	 * Constructor with object mapper.
	 * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
	 */
	Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
	{}
public:
  
	ENDPOINT("GET", "/", root) {
		auto dto = HelloWorld::createShared();
		dto->message = "Hello World!";
		return createDtoResponse(Status::CODE_200, dto);
	}

	/*
	ENDPOINT("DELETE", "/api/v1/ConsumeLicense", ConsumeLicense, BODY_DTO(Object<License>, license)){
		// Assert requires fields are present.
	    OATPP_ASSERT_HTTP(license->name, Status::CODE_400, "Missing field 'name'.");
		OATPP_ASSERT_HTTP(license->time, Status::CODE_400, "Missing field 'time'.");

		license->name = removeWhitespace(license->name);

		// Assert license is in pool and that it has enough time to consume request.
		OATPP_ASSERT_HTTP(pool.find(license->name) != pool.end(), Status::CODE_400, "License not found!");
		OATPP_ASSERT_HTTP(pool[license->name] - license->time >=0, Status::CODE_400, "Time field exceeds license total time pool!");

		// If all is good, subtract time for license.
		pool[license->name] -= license->time;
		writePoolToFile(pool);

		license->time = pool[license->name];
		return createDtoResponse(Status::CODE_200, license);
	}

	*/
	ENDPOINT("GET", "/api/v1/licenses", getLicenses){
		const auto licenses = getPool();
		return createDtoResponse(Status::CODE_200, licenses);
	}

	ENDPOINT("POST", "/api/v1/upload", multiUpload,
			REQUEST(std::shared_ptr<IncomingRequest>, request))
	{

		/* create multipart object */
		multipart::PartList multipart(request->getHeaders());

		/* create multipart reader */
		multipart::Reader multipartReader(&multipart);

		/* setup reader to stream parts to a temporary files by default */
		multipartReader.setDefaultPartReader(multipart::createTemporaryFilePartReader("/tmp" /* /tmp directory */));

		/* upload multipart data */
		request->transferBody(&multipartReader);

		/* list all parts and locations to corresponding temporary files */
		auto parts = multipart.getAllParts();

		// Data for filtering out invalid parts.
		enum Part{intermediate, license, signature};
		std::map<std::string, Part> acceptedParts{{"intermediate", intermediate}, {"license", license}, {"signature", signature}};

		std::string intermediateCert = "";
		std::string licenseFile = "";
		std::string signatureFile = "";
		for(auto& p : parts) {

			// If part is not present in accepted list of parts, continue.
			if(!acceptedParts.count(p->getName()->c_str())){
				continue;
			}

			// Get correct value for part
			Part part = acceptedParts[p->getName()->c_str()];
			auto location = p->getPayload()->getLocation()->c_str();

			switch(part){ 
				case intermediate: intermediateCert = location; break;
				case license: licenseFile = location; break; // TODO:: Add support for reading license file;
				case signature: signatureFile = location; break; // TODO:: Add support for reading signature file;
			}
		}

		// Assert all files present
		OATPP_ASSERT_HTTP(intermediateCert!="", Status::CODE_400, "Certificate could not be found.");
		OATPP_ASSERT_HTTP(licenseFile!="", Status::CODE_400, "License file could not be found.");
		OATPP_ASSERT_HTTP(signatureFile!="", Status::CODE_400, "Signature file could not be found.");

		// Verify root and intermediate certs
		X509 * intCert = readCertFromFile(intermediateCert);
		X509 * rootCert = readCertFromFile("../cert/external/root.cert");
		OATPP_ASSERT_HTTP(cert_verify(intCert, rootCert)==1, Status::CODE_401, "Certificate could not be validated!");
		X509_free(intCert);
		X509_free(rootCert);


		// Derive intermediate pub.key and try to verify siganture against key and license payload.
		int createIntPubKey = system(("openssl x509 -in "+intermediateCert+" -pubkey -noout > intpubkey.pem").c_str());
		int verifySignature = system(("openssl dgst -sha256 -verify intpubkey.pem -signature "+signatureFile+" "+licenseFile).c_str());


		// Assert success of all previous system commands.
		OATPP_ASSERT_HTTP(createIntPubKey==0, Status::CODE_400, "Could not derive public key from certificate.");


		OATPP_ASSERT_HTTP(verifySignature==0, Status::CODE_401, "Could not verify license with license signature.");


		/* create serializer and deserializer configurations */
		auto serializeConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
		auto deserializeConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
		serializeConfig->useBeautifier = true; // Enable beautifier
		auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

		// Parse license json into custom DTO
		std::string str;
		readContents(licenseFile, &str);


		const auto payload = jsonObjectMapper->readFromString<oatpp::Object<SubLicenseFile>>(str);
		const auto keys = payload->license->keys;

		// Assert that the key list is not null
		OATPP_ASSERT_HTTP(keys, Status::CODE_401, "List of license keys not found in payload");

		for(int i = 0; i < keys->size(); i++){
			const auto l = keys[i];
			addLicenseToPool(l);
		}

		/* return 200 */
		return createResponse(Status::CODE_200, "OK");
	}

	// Convert entire pool to DTOs
	LicensePayload::Wrapper getPool(){
		auto licenseList = LicensePayload::createShared();
		oatpp::List<oatpp::Object<License>> list({});
		for(auto p: pool){
			list->emplace(list->end(), p.second);
		}
		licenseList->licenses = list;
		return licenseList;
	}

	void addLicenseToPool(oatpp::Object<License> license){
		if (pool.find(license->name)!= pool.end()){
			auto curr = pool[license->name];
			curr->duration=license->duration+curr->duration;
			pool[license->name] = curr;
		} else {
			pool[license->name] = license;
		}
	}

	std::string removeWhitespace(std::string str){
		str = boost::trim_left_copy(str);  // Remove prefix spaces
    	str = boost::trim_right_copy(str); // Remove suffix spaces
		return str;
	}

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
