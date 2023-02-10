#ifndef Controller_hpp
#define Controller_hpp

#include "dto/DTOs.hpp"

#include "shared.hpp"
#include "file/fileHandler.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/data/stream/FileStream.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <string>
#include <map>

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
