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

void readContents(std::string path, std::string *out){
    
    std::ifstream ifs(path);
    out->assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
    
    std::cout << *out << std::endl;
    
}