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

	
	ENDPOINT("DELETE", "/api/v1/consume", ConsumeLicense, BODY_DTO(Object<LicenseConsumption>, consumption)){
		// Assert requires fields are present.
	    OATPP_ASSERT_HTTP(consumption->mediaFunction, Status::CODE_400, "Missing field 'mediaFunction'.");
		OATPP_ASSERT_HTTP(consumption->duration, Status::CODE_400, "Missing field 'duration'.");

		consumption->mediaFunction = removeWhitespace(consumption->mediaFunction);

		// Assert license is in pool and that it has enough time to consume request.
		OATPP_ASSERT_HTTP(pool.find(consumption->mediaFunction) != pool.end(), Status::CODE_400, "License not found!");
		OATPP_ASSERT_HTTP((pool[consumption->mediaFunction]->duration - consumption->duration) >=0, Status::CODE_400, "Time field exceeds license total time pool!");

		// If all is good, subtract time for license.
		License::Wrapper license = pool[consumption->mediaFunction];
		license->duration = license->duration - consumption->duration;
		pool[consumption->mediaFunction] = license;

		return createDtoResponse(Status::CODE_200, license);
	}

	
	ENDPOINT("GET", "/api/v1/licenses", getLicenses){
		const auto licenses = getPool();
		return createDtoResponse(Status::CODE_200, licenses);
	}

	ENDPOINT("POST", "/api/v1/upload", multiUpload,
			REQUEST(std::shared_ptr<IncomingRequest>, request))
	{
		std::string certificate = "";
		std::string licenseFile = "";
		std::string signatureFile = "";

		// list of all parts
		auto parts = multiPart(request);
		getCorrectParts(parts, &certificate, &licenseFile, &signatureFile);
		
		// Assert all files present
		OATPP_ASSERT_HTTP(licenseFile!="", Status::CODE_400, "License file could not be found.");
		OATPP_ASSERT_HTTP(signatureFile!="", Status::CODE_400, "Signature file could not be found.");

		// Treat top- and sub-licenses differently due to chain of trust.
		if( certificate != ""){
			// Verify intermediate cert as derived from root.
			X509 * intCert = readCertFromFile(certificate);
			X509 * rootCert = readCertFromFile("../cert/external/root.cert");
			OATPP_ASSERT_HTTP(cert_verify(intCert, rootCert)==1, Status::CODE_401, "Certificate could not be validated!");
			X509_free(intCert);
			X509_free(rootCert);
		} else {
			// Fetch root for certificate
			certificate = "../cert/external/root.cert";
		}

		// Derive intermediate pub.key and try to verify siganture against key and license payload.
		int createIntPubKey = system(("openssl x509 -in "+certificate+" -pubkey -noout > intpubkey.pem").c_str());
		int verifySignature = system(("openssl dgst -sha256 -verify intpubkey.pem -signature "+signatureFile+" "+licenseFile).c_str());

		// Assert success of all previous system commands.
		OATPP_ASSERT_HTTP(createIntPubKey==0, Status::CODE_400, "Could not derive public key from certificate.");
		OATPP_ASSERT_HTTP(verifySignature==0, Status::CODE_401, "Could not verify license with license signature.");

		// Parse license json into custom DTO
		std::string str;
		readContents(licenseFile, &str);
		const auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
		const auto payload = jsonObjectMapper->readFromString<oatpp::Object<SubLicenseFile>>(str);
		const auto keys = payload->license->keys;

		// Assert requires data fields
		OATPP_ASSERT_HTTP(payload, Status::CODE_401, "Valid payload not found.");
		OATPP_ASSERT_HTTP(payload->license, Status::CODE_401, "License not found in payload.");
		OATPP_ASSERT_HTTP(payload->license->keys, Status::CODE_401, "List of license keys not found in payload.");

		// Add all found licenses to pool(s). Will only be one license for sub-licenses and possibly multiple licenses for top-level licenses.
		for(int i = 0; i < keys->size(); i++){
			const auto l = keys[i];
			addLicenseToPool(l);
		}

		// Return 200.
		return createResponse(Status::CODE_200, "OK");
	}

	// Get all parts out of multipart request
	std::list<std::shared_ptr<multipart::Part>> multiPart(const std::shared_ptr<IncomingRequest> &request){
		// Set up multipart reader
		multipart::PartList multipart(request->getHeaders());
		multipart::Reader multipartReader(&multipart);
		multipartReader.setDefaultPartReader(multipart::createTemporaryFilePartReader("/tmp" /* /tmp directory */));
		request->transferBody(&multipartReader);

		// list of all parts
		return multipart.getAllParts();
	}

	// Extract multipart temp file paths from multipart list.
	void getCorrectParts(const std::list<std::shared_ptr<multipart::Part>> parts, std::string *intermediateCert, std::string *licenseFile, std::string *signatureFile){
		// Data for filtering out invalid parts.
		enum Part{intermediate, license, signature};
		std::map<std::string, Part> acceptedParts{{"intermediate", intermediate}, {"license", license}, {"signature", signature}};

		for(auto& p : parts) {

			// If part is not present in accepted list of parts, continue.
			if(!acceptedParts.count(p->getName()->c_str())){
				continue;
			}

			// Get correct value for part
			Part part = acceptedParts[p->getName()->c_str()];
			auto location = p->getPayload()->getLocation()->c_str();

			// Assign path to correct variable using Part enum.
			switch(part){ 
				case intermediate: *intermediateCert = location; break;
				case license: *licenseFile = location; break;
				case signature: *signatureFile = location; break;
			}
		}
	}

	// Convert pool License DTO list.
	LicensePayload::Wrapper getPool(){
		auto licenseList = LicensePayload::createShared();
		oatpp::List<oatpp::Object<License>> list({});
		for(auto p: pool){
			list->emplace(list->end(), p.second);
		}
		licenseList->licenses = list;
		return licenseList;
	}

	// Add license to pool / create new pool from license.
	void addLicenseToPool(oatpp::Object<License> license){
		if (pool.find(license->name)!= pool.end()){ // License is in pool
			auto curr = pool[license->name];
			curr->duration=license->duration+curr->duration;
			pool[license->name] = curr;
		} else { // License is not already in pool... Add it.
			pool[license->name] = license; 
		}
	}

	// Cut away whitespace from string.
	std::string removeWhitespace(std::string str){
		str = boost::trim_left_copy(str);  // Remove prefix spaces
    	str = boost::trim_right_copy(str); // Remove suffix spaces
		return str;
	}

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
