#ifndef fileHandler_HPP
#define fileHandler_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "error/error.hpp"

#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

void readContents(std::string path, std::string *out);
X509 * readCertFromFile(std::string path);

#endif