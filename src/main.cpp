// Standard headers
#include <iostream>
#include <string>
#include <map>

// Custom headers
#include "crow.h"
#include "API.h"

std::map<std::string, int> pool;

int main(){
    
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });


    CROW_ROUTE(app, "/license")
    .methods("POST"_method , "DELETE"_method )
    ([](const crow::request& req) {
        return license(req);
    });

    app.port(8080).run();
}
