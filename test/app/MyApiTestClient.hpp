
#ifndef MyApiTestClient_hpp
#define MyApiTestClient_hpp

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
class MyApiTestClient : public oatpp::web::client::ApiClient {

	API_CLIENT_INIT(MyApiTestClient)

	API_CALL("GET", "/", getRoot)

	// TODO - add more client API calls here

};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)

#endif // MyApiTestClient_hpp
