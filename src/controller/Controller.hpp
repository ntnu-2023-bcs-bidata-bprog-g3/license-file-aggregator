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
    pool[license->name] += license->time;

    //std::cout<< license->name << " " << license->time << std::endl;
    oatpp::List<oatpp::Object<License>> list({});
    for(auto p: pool){
      auto l = License::createShared();
      l->name = p.first;
      l->time = p.second;
      list->emplace(list->end(), l);
    }
    auto licenseList = LicenseList::createShared();
    licenseList->licenses = list;

    return createDtoResponse(Status::CODE_200, licenseList);
  }
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
