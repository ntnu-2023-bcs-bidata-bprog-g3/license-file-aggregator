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
  
	ENDPOINT("POST", "/AddLicense", AddLicense, BODY_DTO(Object<License>, license)){
		// Assert required fields are present.
    	OATPP_ASSERT_HTTP(license->name, Status::CODE_400, "Missing field 'name'.");
		OATPP_ASSERT_HTTP(license->time, Status::CODE_400, "Missing field 'time'.");

		license->name = removeWhitespace(license->name);

		// Assert fields have valid data.
		OATPP_ASSERT_HTTP(license->name!="", Status::CODE_400, "Name field cannot be empty.");
		OATPP_ASSERT_HTTP(license->time > 0, Status::CODE_400, "Time must be greater than 0.")

		// If all is good, add time for license.
		pool[license->name] += license->time;
		writePoolToFile(pool);

		license->time = pool[license->name];
		return createDtoResponse(Status::CODE_200, license);
	}

	ENDPOINT("DELETE", "/ConsumeLicense", ConsumeLicense, BODY_DTO(Object<License>, license)){
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

	ENDPOINT("GET", "/licenses", getLiceses){
		const auto licenses = getPool();
		return createDtoResponse(Status::CODE_200, licenses);
	}

	ENDPOINT("POST", "/upload", upload, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
		oatpp::data::stream::FileOutputStream fileOutputStream("test123.txt");
		request->transferBodyToStream(&fileOutputStream); // transfer body chunk by chunk
		return createResponse(Status::CODE_200, "OK");
	}

	ENDPOINT("POST", "/upload/multipart", multiUpload,
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

		X509 * intermediateCert = NULL;
		//TODO:: Signature variable
		//TODO:: License variable
		for(auto& p : parts) {

			// If part is not present in accepted list of parts, continue.
			if(!acceptedParts.count(p->getName()->c_str())){
				continue;
			}

			// Get correct value for part
			Part part = acceptedParts[p->getName()->c_str()];
			auto location = p->getPayload()->getLocation()->c_str();

			switch(part){ 
				case intermediate: intermediateCert = readCertFromFile(location); break;
				case license: break; // TODO:: Add support for reading license file;
				case signature: break; // TODO:: Add support for reading signature file;
			}
		}

		OATPP_ASSERT_HTTP(intermediateCert, Status::CODE_400, "Certificate could not be found");
		//TODO:: Assert license
		//TODO:: Assert signature

		X509 * rootCert = readCertFromFile("../cert/external/root.cert");
		OATPP_ASSERT_HTTP(sig_verify(intermediateCert, rootCert)==1, Status::CODE_401, "Certificate could not be validated!");

		X509_free(intermediateCert);

		/* return 200 */
		return createResponse(Status::CODE_200, "OK");

	}

	// Convert entire pool to DTOs
	LicenseList::Wrapper getPool(){
		auto licenseList = LicenseList::createShared();
		oatpp::List<oatpp::Object<License>> list({});
		for(auto p: pool){
			auto l = getLicenseDTO(p);
			list->emplace(list->end(), l);
		}
		licenseList->licenses = list;
		return licenseList;
	}

	License::Wrapper getLicenseDTO(std::pair<std::string, int> license){
		auto l = License::createShared();
		l->name = license.first;
		l->time = license.second;
		return l;
	}

	std::string removeWhitespace(std::string str){
		str = boost::trim_left_copy(str);  // Remove prefix spaces
    	str = boost::trim_right_copy(str); // Remove suffix spaces
		return str;
	}

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
