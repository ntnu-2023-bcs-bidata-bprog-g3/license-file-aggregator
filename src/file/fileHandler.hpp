#ifndef fileHandler_HPP
#define fileHandler_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <streambuf>
#include <optional>

#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

void writePoolToFile(std::map<std::string, int> pool);
void readPoolFromFile(std::map<std::string,int> *pool);
void readContents(std::string path, std::string *out);
X509 * readCertFromFile(std::string path);

#endif