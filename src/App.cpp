#include "./controller/Controller.hpp"
#include "./AppComponent.hpp"
#include "shared.hpp"
#include "file/fileHandler.hpp"
#include "ssl/certificates.hpp"
#include "error/error.hpp"

#include "oatpp/network/Server.hpp"

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "client/client.hpp"

#include <iostream>
std::unordered_map<std::string, License::Wrapper> pool;

void run() {

	/* Register Components in scope of run() method */
	AppComponent components;

	/* Get router component */
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

	/* Create MyController and add all of its endpoints to router */
	router->addController(std::make_shared<Controller>());

	/* Get connection handler component */
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

	/* Get connection provider component */
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

	/* Create server which takes provided TCP connections and passes them to HTTP connection handler */
	oatpp::network::Server server(connectionProvider, connectionHandler);

	/* Print info about server port */
	OATPP_LOGI("License_consumer", "Server running on port %s", connectionProvider->getProperty("port").getData());

	server.run();
  
}

void request(){
	using namespace oatpp::network;
	using namespace oatpp::web;
	using namespace oatpp::parser;
	// Create connection provider

	// Change url and port to NMS static IP
	//TODO:: Detect if in prod or dev enc and use either local or proper nms service IPs
	auto connectionProvider = tcp::client::ConnectionProvider::createShared({"10.24.103.222", 8090, oatpp::network::Address::IP_4});	

	// Create httpRequestExecutor & ObjectMapper
	auto requestExecutor = client::HttpRequestExecutor::createShared(connectionProvider);		
	auto objectMapper = json::mapping::ObjectMapper::createShared();

	// Create client
	auto client = Client::createShared(requestExecutor, objectMapper);

	auto data = client->getResource("TEST123");
	if (data->getStatusCode() == 404) {
		std::cout << "COULD NOT FIND SERVER" << std::endl;
		// TODO:: Return out with error msg or something.
	} 

	// Showing that we can fetch response data if needed. (Not really needed for project)
	std::cout << data->getStatusCode() << std::endl << data->readBodyToString()->c_str() << std::endl;
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {

	try{
		request();
	} catch(...){
		std::cerr<<"Could not connect to NMS. Assuming there is no NMS in network..." << std::endl;
	}	

	OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests(); 

	// Check that root cert can be parsed.
	X509 * root = readCertFromFile("../cert/external/root.cert");
	//TODO:: Add check that cert was read successfully. Otherwise exit program.
	X509_free(root);

	oatpp::base::Environment::init();

	run();
		
	/* Print how much objects were created during app running, and what have left-probably leaked */
	/* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
	std::cout << "\nEnvironment:\n";
	std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
	std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
	
	oatpp::base::Environment::destroy();
	
	return 0;
}
