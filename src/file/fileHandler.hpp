#ifndef fileHandler_HPP
#define fileHandler_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <streambuf>

#include <openssl/x509v3.h> //x509 implementation for compatibility
#include <openssl/bn.h> // 
#include <openssl/asn1.h>
#include <openssl/x509.h> // x509 implementation
#include <openssl/x509_vfy.h> 
#include <openssl/pem.h> // for reading certificates & keys
#include <openssl/bio.h>

void writePoolToFile(std::map<std::string, int> pool);
void readPoolFromFile(std::map<std::string,int> * pool);
void readContents(std::string path, std::string *out);
void readRootCertFromFile();
#endif