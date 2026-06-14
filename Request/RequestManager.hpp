#pragma once
namespace Request {
    class RequestManager {
    public:
        /// @brief 
        /// @return A singleton of the request manager
        static const RequestManager& getSingleton();
    };
} // Request