#ifndef error_hpp
#define error_hpp

#include "dto/DTOs.hpp"


// Create new error response
Error::Wrapper err(std::string msg){
    auto error = Error::createShared();
    error->errormsg = msg;
    return error;
}

#endif /*error.hpp*/