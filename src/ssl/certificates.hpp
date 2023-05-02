#ifndef certificates_HPP
#define certificates_HPP

#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include <string>

int cert_verify(X509* intermediate, X509* root);

#endif /* certificates_HPP */