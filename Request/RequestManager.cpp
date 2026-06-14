#include "RequestManager.hpp"

namespace Request {
    const RequestManager& RequestManager::getSingleton() {
        static const RequestManager manager;
        return manager;
    }
}