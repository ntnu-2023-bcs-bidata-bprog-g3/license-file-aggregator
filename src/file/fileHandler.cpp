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

std::string readRootCertFromFile(){
    std::ifstream ifs("../cert/root_cert.crt");
    std::string content;
    content.assign( (std::istreambuf_iterator<char>(ifs) ),
                    (std::istreambuf_iterator<char>()    ) );
    return content;
}