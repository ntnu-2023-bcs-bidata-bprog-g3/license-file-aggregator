#include "API.h"

extern std::map<std::string, int> pool;

crow::response license(crow::request req){
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
}

crow::response getLicense(crow::request req){
    using namespace std;

    const string params[] = {"id"};
    map<string,string> vals = param2map(req.url_params, params, params->length());
    if(!inMap(vals, (string)"id"))
        return crow::response(map2JSON(pool));
    return crow::response(inMap(pool, vals["id"]) ? val2JSON(vals["id"], pool[vals["id"]]) : 404);
}

template <typename T>
void print(T str){
    std::cout << str << std::endl;
}

int poolSubtract(const std::string type, const int time){
    return pool[type] - time > 0 ? time : 0;
}


//####################################
// TODO:: Separate into own file
template <typename T>
crow::json::wvalue map2JSON(std::map<std::string, T> map){
    crow::json::wvalue json;
    for(const auto x: map)
        json[x.first] = x.second;
    return json;
}

template <typename T>
crow::json::wvalue val2JSON(const std::string key, const T val){
    crow::json::wvalue json;
    json[key] = val;
    return json;
}

// Checks if key is in map where key and value is not the same
template <typename T, typename U>
bool inMap(std::map<U, T> map, const U key){
    return map.find(key) != map.end();
}

template <typename T>
bool inMap(std::map<T, T> map, const T key){
    return map.find(key) != map.end();
}

// Parse url params and get all values into map.
std::map<std::string,std::string> param2map(crow::query_string url, const std::string params[], const int size){
    std::map<std::string, std::string> vals;
    for(int i = 0; i<size-1; i++){
        const auto param = params[i];
        if(url.get(param) == nullptr) continue;
        vals[param] = url.get(param);
    }
    return vals;
}