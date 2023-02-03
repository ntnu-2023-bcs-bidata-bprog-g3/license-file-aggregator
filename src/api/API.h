#ifndef API_H
#define API_H

#include "crow.h"
#include "fileHandler.h"

crow::response license(crow::request req);
crow::response getLicense(crow::request req);

int poolSubtract(std::string type, int time);
template <typename T> void print(T str);

//####################################
// TODO:: Separate into own file
template <typename T> crow::json::wvalue map2JSON(std::map<std::string, T> map);
template <typename T> crow::json::wvalue val2JSON(const std::string key, const T val);
template <typename T> std::optional<T> inMap(std::map<T, T> map, const T key);
template <typename T, typename U> std::optional<U> inMap(std::map<T, U> map, const T key);

std::map<std::string,std::string> param2map(crow::query_string url, const std::string params[], const int size);

#endif