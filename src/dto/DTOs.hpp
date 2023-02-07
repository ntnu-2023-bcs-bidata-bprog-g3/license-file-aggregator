#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class HelloWorld : public oatpp::DTO {
  
  DTO_INIT(HelloWorld, DTO)
  
  DTO_FIELD(String, message);
  
};

class License : public oatpp::DTO{
  DTO_INIT(License, DTO)

  DTO_FIELD(String, name);
  DTO_FIELD(Int32, time);
};

class LicenseList : public oatpp::DTO{
  DTO_INIT(LicenseList, DTO)

  DTO_FIELD(List<Object<License>>, licenses);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
