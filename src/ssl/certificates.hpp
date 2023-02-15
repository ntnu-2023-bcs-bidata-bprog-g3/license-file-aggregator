#ifndef certificates_HPP
#define certificates_HPP

#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include <string>

int sig_verify(const std::string cert_pem, const std::string intermediate_pem);
void cert_info(const char* cert_pem);

#endif /* certificates_HPP */