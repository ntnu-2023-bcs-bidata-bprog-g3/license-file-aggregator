#ifndef fileHandler_HPP
#define fileHandler_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>

void writePoolToFile(std::map<std::string, int> pool);
void readPoolFromFile(std::map<std::string,int> * pool);
std::string readRootCertFromFile();
#endif