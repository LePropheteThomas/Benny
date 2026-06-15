#pragma once
#include "RequestResponse.hpp"

#include <map>
#include <string>

namespace Request {
    struct RequestResponse {
        std::map<std::string, std::string> headers;
        std::string body;
        unsigned int statusCode;

        RequestResponse(): statusCode(0) {};
    };
}
