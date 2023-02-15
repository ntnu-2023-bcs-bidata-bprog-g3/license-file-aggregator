#include "certificates.hpp"

int sig_verify(const std::string cert_pem, const std::string intermediate_pem)
{

    // intermediate
    char intermediate[intermediate_pem.length()];
    strcpy(intermediate, intermediate_pem.c_str());

    BIO *b = BIO_new(BIO_s_mem());
    BIO_puts(b, intermediate);
    X509 * issuer = PEM_read_bio_X509(b, NULL, NULL, NULL);
    EVP_PKEY *signing_key=X509_get_pubkey(issuer);

    // root
    char cert[cert_pem.length()];
    strcpy(cert, cert_pem.c_str());

    BIO *c = BIO_new(BIO_s_mem());
    BIO_puts(c, cert);
    X509 * x509 = PEM_read_bio_X509(c, NULL, NULL, NULL);
 
    // Verify
    int result = X509_verify(x509, signing_key);
 
    EVP_PKEY_free(signing_key);
    BIO_free(b);
    BIO_free(c);
    X509_free(x509);
    X509_free(issuer);
 
    return result;
}

/*
void cert_info(const char* cert_pem)
{
    BIO *b = BIO_new(BIO_s_mem());
    BIO_puts(b, cert_pem);
    X509 * x509 = PEM_read_bio_X509(b, NULL, NULL, NULL);
 
    BIO *bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);
 
    //Subject
    BIO_printf(bio_out,"Subject: ");
    X509_NAME_print(bio_out,X509_get_subject_name(x509),0);
    BIO_printf(bio_out,"\n");
 
    //Issuer
    BIO_printf(bio_out,"Issuer: ");
    X509_NAME_print(bio_out,X509_get_issuer_name(x509),0);
    BIO_printf(bio_out,"\n");
 
    //Public Key
    EVP_PKEY *pkey=X509_get_pubkey(x509);
    EVP_PKEY_print_public(bio_out, pkey, 0, NULL);
    EVP_PKEY_free(pkey);
 
    //Signature
    X509_signature_print(bio_out, x509->sig_alg, x509->signature);
    BIO_printf(bio_out,"\n");
 
    BIO_free(bio_out);
    BIO_free(b);
    X509_free(x509);
}
*/