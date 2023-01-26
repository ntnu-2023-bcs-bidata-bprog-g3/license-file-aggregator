#include <iostream>
#include <openssl/x509v3.h> //x509 implementation for compatibility
#include <openssl/bn.h> // 
#include <openssl/asn1.h>
#include <openssl/x509.h> // x509 implementation
#include <openssl/x509_vfy.h> 
#include <openssl/pem.h> // for reading certificates & keys
#include <openssl/bio.h>

void openCert();

int main(){

    openCert();

    std::cout << "Hello world! PP" << std::endl;

    return 0;
}

void openCert(){
    std::string pathToCert = "C:\\your\\path\\to\\certificate.pem";
    FILE* fp = fopen(pathToCert.c_str(), "r");
    if (!fp) {
        std::cout << "Could not open certificate";
        return;
    }X509* cert = PEM_read_X509(fp, NULL, NULL, NULL);if (!cert) {
        std::cout << "Could not parse certificate";
        return;
    }
    fclose(fp);// certificate opened successfully
    // do something with `cert`
    
    // Free memory and prevent memory leaks
    X509_free(cert);
}