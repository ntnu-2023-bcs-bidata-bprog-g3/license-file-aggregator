#ifndef API_H
#define API_H

#include "crow.h"

int poolSubtract(std::string type, int time);
template <typename T> crow::json::wvalue map2JSON(std::map<std::string, T> map);
crow::response license(crow::request req);

#endif