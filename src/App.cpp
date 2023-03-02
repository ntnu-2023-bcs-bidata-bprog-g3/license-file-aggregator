#include "./controller/Controller.hpp"
#include "./AppComponent.hpp"
#include "shared.hpp"
#include "file/fileHandler.hpp"
#include "ssl/certificates.hpp"
#include "error/error.hpp"

#include "oatpp/network/Server.hpp"

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

/**
 *  main
 */
int main(int argc, const char * argv[]) {

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
