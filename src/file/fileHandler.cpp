#include "fileHandler.hpp"

void readContents(std::string path, std::string *out){
    
    std::ifstream ifs(path);
    out->assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
}

X509 * readCertFromFile(std::string path){
    
    // Open file
    FILE* fp = fopen(path.c_str(), "r");
    if(!fp){
        err("File with path ["+path+"]. Not found.");
        return NULL;
    }

    // Read certificate
    X509 * cert = PEM_read_X509(fp, NULL, 0, NULL);
    if(!cert){
        err("Cert at ["+path+"] could not be read sucessfully.");
        return NULL;
    }

    // Close file
    int result = fclose(fp);
    if (result != 0) {
        err("Could not close file at ["+path+"] properly.");
        return NULL;
    }

    return cert;
}