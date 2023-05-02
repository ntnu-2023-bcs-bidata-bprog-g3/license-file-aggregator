#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"

#include "oatpp-openssl/server/ConnectionProvider.hpp"

#include "oatpp-openssl/Config.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:
  
	/**
	 *  Create ConnectionProvider component which listens on the port
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
		const char* pemFile = "../cert/local/key.pem";
		const char* crtFile = "../cert/local/cert.crt";

		auto config = oatpp::openssl::Config::createDefaultServerConfigShared(crtFile, pemFile);
		auto connectionProvider = oatpp::openssl::server::ConnectionProvider::createShared(config, {"0.0.0.0", 8443});
		return connectionProvider;
	}());
  
	/**
	 *  Create Router component
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
		return oatpp::web::server::HttpRouter::createShared();
	}());
	
	/**
	 *  Create ConnectionHandler component which uses Router component to route requests
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
		return oatpp::web::server::HttpConnectionHandler::createShared(router);
	}());
	
	/**
	 *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
		return oatpp::parser::json::mapping::ObjectMapper::createShared();
	}());

};

#endif /* AppComponent_hpp */
