#pragma once
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


    };
} // Request