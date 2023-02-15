#ifndef fileHandler_HPP
#define fileHandler_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <streambuf>

void writePoolToFile(std::map<std::string, int> pool);
void readPoolFromFile(std::map<std::string,int> * pool);
void readContents(std::string path, std::string *out);
#endif