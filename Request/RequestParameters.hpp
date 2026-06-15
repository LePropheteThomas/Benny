#pragma once
#include <string>
#include <utility>

namespace Request {
    enum class RequestMethods {
        POST, ///< POST request
        GET, ///< GET request
        WSS ///< WSS request
    };

    struct RequestParameters {
    protected:
        std::string url; ///< The URL for the request
        RequestMethods method; ///< The type of the request

        /// @brief Protected constructor to make inheritance possible
        /// @param url The URL for the request
        /// @param method The type for the request
        RequestParameters(std::string url, const RequestMethods method): url(std::move(url)), method(method) {};

    public:
        /// @brief Default constructor
        /// @tparam M The type of the normal request (ex.: Request::RequestMethods::GET)
        /// @param url The URL of the request
        template <RequestMethods M>
        requires (M != RequestMethods::WSS)
        explicit RequestParameters(std::string url): RequestParameters(std::move(url), M) {};
    };

    struct WSSRequestParameters : RequestParameters {
        /// @brief Default constructor
        /// @param url The URL for the WSS request
        explicit WSSRequestParameters(std::string url): RequestParameters(std::move(url), RequestMethods::WSS) {}
    };
}
