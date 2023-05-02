#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiClient) ///< Begin code-gen section

class Client : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(Client)
  // TODO:: Change to correct endpoint. Consult Sander on post body etc.
  API_CALL("PUT", "api/v1/lfa/register", getResource, QUERY(String, name), QUERY(String, port))

};

#include OATPP_CODEGEN_END(ApiClient) ///< End code-gen section
