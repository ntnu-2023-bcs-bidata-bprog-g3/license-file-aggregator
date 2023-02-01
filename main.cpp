#include <iostream>
#include "crow.h"
#include <string>
#include <map>

crow::json::wvalue poolJSON();
template <typename T>
crow::json::wvalue map2JSON(std::map<std::string, T> map);

std::map<std::string, int>pool;

int main(){
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/fill")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/fill")
    .methods("POST"_method)
    ([](const crow::request& req) {
        if (req.method != "POST"_method)
            return crow::response(404);
        
        auto license = crow::json::load(req.body);

        if (!license)
            return crow::response(400);
        
        std::string type = license["type"].s();
        int time = license["time"].i(); 

        pool[type] += time;

        return crow::response(map2JSON(pool));
    });



    app.port(8080).run();
}

template <typename T>
crow::json::wvalue map2JSON(std::map<std::string, T> map){
    crow::json::wvalue json;
    for(const auto x: pool)
        json[x.first] = x.second;
    return json;
}