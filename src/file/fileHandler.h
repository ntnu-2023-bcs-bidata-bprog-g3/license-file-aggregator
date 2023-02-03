#ifndef fileHandler_H
#define fileHandler_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>

void writePoolToFile(std::map<std::string, int> pool);
void readPoolFromFile(std::map<std::string,int> * pool);

#endif