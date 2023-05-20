
#ifndef ApiTestClient_hpp
#define ApiTestClient_hpp

#include "dto/DTOs.hpp"
#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include <map>
/* Begin Api Client code generation */
#include OATPP_CODEGEN_BEGIN(ApiClient)

std::unordered_map<std::string, License::Wrapper> pool;
std::string name;

/**
 * Test API client.
 * Use this client to call application APIs.
 */
class ApiTestClient : public oatpp::web::client::ApiClient {

	API_CLIENT_INIT(ApiTestClient)

	API_CALL("GET", "/api/v1/", getRoot)

	// TODO - add more client API calls here

};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)

#endif // MyApiTestClient_hpp
