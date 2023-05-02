#include "certificates.hpp"

int cert_verify(X509* intermediate, X509* root)
{
    EVP_PKEY *signing_key = X509_get_pubkey(root);
    return X509_verify(intermediate, signing_key); 
}