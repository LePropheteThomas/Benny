#pragma once
#include <utility>

#include "../RequestParameters.hpp"
#include "curl/curl.h"

namespace Request::Transactions {
    class WebsocketTransaction {
    private:
        WSSRequestParameters parameters; ///< The websocket request parameters
        CURL* curl; ///< CURL object for websocket

    public:
        /// @brief Simple constructor
        /// @param parameters The websocket request parameters
        /// @param curl CURL object for websocket
        WebsocketTransaction(WSSRequestParameters parameters, CURL* curl): parameters(std::move(parameters)), curl(curl) {}
    };
}
