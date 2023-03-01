#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiClient) ///< Begin code-gen section

class Client : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(Client)

  API_CALL("GET", "/resource", getResource)

};

#include OATPP_CODEGEN_END(ApiClient) ///< End code-gen section
