#include "certificates.hpp"

int sig_verify(X509* intermediate, X509* root)
{
    EVP_PKEY *signing_key = X509_get_pubkey(root);
    return X509_verify(intermediate, signing_key); 
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