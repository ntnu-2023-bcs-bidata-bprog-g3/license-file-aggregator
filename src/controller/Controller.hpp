#ifndef Controller_hpp
#define Controller_hpp

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <iostream>
#include <string>
#include <map>
std::map<std::string, int> pool;
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
		if(!license->name || !license->time){
			auto response = HelloWorld::createShared();
			response->message = "Invalid request body.";
			return createDtoResponse(Status::CODE_400, response);
		} 

		pool[license->name] += license->time;

		auto licenseList = getPool();

		return createDtoResponse(Status::CODE_200, licenseList);
	}

	ENDPOINT("DELETE", "/ConsumeLicense", ConsumeLicense, BODY_DTO(Object<License>, license)){
		// Check if requires fields are present.
		if(!license->name || !license->time){
			auto response = HelloWorld::createShared();
			response->message = "Invalid request body.";
			return createDtoResponse(Status::CODE_400, response);
		} 

		pool[license->name] -= license->time;

		auto licenseList = getPool();

		return createDtoResponse(Status::CODE_200, licenseList);
	}

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

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
