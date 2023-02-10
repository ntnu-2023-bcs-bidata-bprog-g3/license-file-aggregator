#include "fileHandler.hpp"

void writePoolToFile(std::map<std::string, int> pool){
    std::ofstream p_file("PoolLedger.DAT");
    for(auto p: pool){
        p_file << p.first << "|" << p.second << std::endl;
    }
    p_file.close();
}
 
void readPoolFromFile(std::map<std::string,int> * pool){
    std::ifstream p_file("PoolLedger.DAT");
    std::string name;
    int time;
    while (getline (p_file, name, '|')) {
        p_file >> time;
        p_file.ignore( 1, '\n');
        pool->insert(std::pair<std::string, int>(name,time));
    }
}

void readRootCertFromFile(){
    std::ifstream ifs("../cert/root_cert.crt");

    std::string pathToCert = "../cert/root_cert.crt";
    FILE* fp = fopen(pathToCert.c_str(), "r");
    if (!fp) {
        std::cout << "Could not open certificate\n";
        return;
    }
    X509* cert = PEM_read_X509(fp, NULL, NULL, NULL);
    if (!cert) {
        std::cout << "Could not parse certificate\n";
        return;
    }

    fclose(fp);//certificate opened successfully
    
    //char *subj = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);
    //char *issuer = X509_NAME_oneline(X509_get_issuer_name(cert), NULL, 0);
    //std::cout << subj << " " << issuer << std::endl;
    
    X509_free(cert);
}