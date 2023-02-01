#include <iostream>
#include "crow.h"
#include <string>
#include <map>

template <typename T> crow::json::wvalue map2JSON(std::map<std::string, T> map);
crow::json::wvalue poolJSON();
int poolSubtract(std::string type, int time);

std::map<std::string, int>pool;

int main(){
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });


    CROW_ROUTE(app, "/license")
    .methods("POST"_method , "DELETE"_method )
    ([](const crow::request& req) {
        auto license = crow::json::load(req.body);

        if (!license)
            return crow::response(400);
        
        std::string type = license["type"].s();
        int time = license["time"].i(); 

        switch(req.method){
            case "POST"_method : pool[type] += time; break;
            case "DELETE"_method : pool[type] -= poolSubtract(type, time); break;
        }

        return crow::response(map2JSON(pool));
    });

    app.port(8080).run();
}

int poolSubtract(std::string type, int time){
    return pool[type] - time > 0 ? time : 0;
}

template <typename T>
crow::json::wvalue map2JSON(std::map<std::string, T> map){
    crow::json::wvalue json;
    for(const auto x: pool)
        json[x.first] = x.second;
    return json;
}