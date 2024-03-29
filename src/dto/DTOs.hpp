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

class LicenseConsumption : public oatpp::DTO{
	DTO_INIT(LicenseConsumption, DTO)

	DTO_FIELD(String, mediaFunction);
	DTO_FIELD(Int32, duration);
};

class License : public oatpp::DTO{
	DTO_INIT(License, DTO)

	DTO_FIELD(String, name);
	DTO_FIELD(Int32, duration);
	DTO_FIELD(String, description);
};


class LicenseList : public oatpp::DTO{
	DTO_INIT(LicenseList, DTO)

	DTO_FIELD(List<Object<License>>, keys);
};


class LicensePayload : public oatpp::DTO{
	DTO_INIT(LicensePayload, DTO)

	DTO_FIELD(List<Object<License>>, licenses);
};

class SubLicenseFile : public oatpp::DTO{
	DTO_INIT(SubLicenseFile, DTO)

	DTO_FIELD(String, name);
	DTO_FIELD(Fields<String>, info);
	DTO_FIELD(Object<LicenseList>, license);
};



#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
