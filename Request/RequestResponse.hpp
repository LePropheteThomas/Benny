#pragma once
#include "RequestResponse.hpp"

#include <map>
#include <string>

namespace Request {
    struct RequestResponse {
        std::map<std::string, std::string> headers; ///< The response headers
        std::string body; ///< The body of the response
        unsigned int statusCode; ///< The status code of the response

        RequestResponse(): statusCode(0) {};
    };
}
