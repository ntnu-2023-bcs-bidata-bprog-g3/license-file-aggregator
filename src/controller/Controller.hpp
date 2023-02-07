#ifndef Controller_hpp
#define Controller_hpp

#include "dto/DTOs.hpp"

#include "error.hpp"
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
		// Check if required fields are present.
		if(!license->name || !license->time)
			return createDtoResponse(Status::CODE_400, err("Invalid request body"));

		license->name = removeWhitespace(license->name);

		// Check if name is filled and if license is in pool
		if(license->name == "" || license->time == 0)
			return createDtoResponse(Status::CODE_400, err("Name and time requires data."));

		// If all is good, add time for license.
		pool[license->name] += license->time;
		writePoolToFile(pool);

		const auto licenseList = getPool();

		return createDtoResponse(Status::CODE_200, licenseList);
	}

	ENDPOINT("DELETE", "/ConsumeLicense", ConsumeLicense, BODY_DTO(Object<License>, license)){
		// Check if requires fields are present.
		if(!license->name || !license->time)
			return createDtoResponse(Status::CODE_400, err("Invalid request body"));

		license->name = removeWhitespace(license->name);

		// Check if name is filled and if license is in pool
		if(pool.find(license->name) == pool.end())
			return createDtoResponse(Status::CODE_400, err("License not found!"));

		// Check if license has enough time left to subtract amount.
		if(pool[license->name] - license->time < 0){
			auto msg = "License with name \'" + license->name + "\' does not have enough time left to consume ["
			+std::to_string(license->time)+"] seconds. (["+std::to_string(pool[license->name])+"] seconds left.)";
			return createDtoResponse(Status::CODE_400, err(msg));
		}

		// If all is good, subtract time for license.
		pool[license->name] -= license->time;
		writePoolToFile(pool);

		const auto licenseList = getPool();

		return createDtoResponse(Status::CODE_200, licenseList);
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
			auto l = License::createShared();
			l->name = p.first;
			l->time = p.second;
			list->emplace(list->end(), l);
		}
		licenseList->licenses = list;
		return licenseList;
	}

	std::string removeWhitespace(std::string str){
		str = boost::trim_left_copy(str);  // Remove prefix spaces
    	str = boost::trim_right_copy(str); // Remove suffix spaces
		return str;
	}

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
