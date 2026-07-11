#pragma once
#include "RequestParameters.hpp"
#include "RequestResponse.hpp"
#include "curl/curl.h"
#include "curl/multi.h"

namespace Request {
    class RequestManager {
    private:
        CURLM* curlm; ///< multiCURL object for pooling and management

    protected:
        /// @brief Simple constructor
        RequestManager() {
            curl_global_init(CURL_GLOBAL_ALL);
            curlm = curl_multi_init();
        }
    public:
        /// @brief
        /// @return A singleton of the request manager
        static const RequestManager& getSingleton();

        const RequestResponse& performHTTPRequest(const RequestParameters& parameters) {
            CURL* handle = curl_easy_init();
            curl_easy_setopt(handle, CURLOPT_URL, parameters.getURL().c_str());
            curl_easy_setopt(handle, CURL_)
        }
    };
} // Request