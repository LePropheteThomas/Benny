#pragma once
#include "RequestParameters.hpp"
#include "RequestResponse.hpp"
#include "curl/curl.h"
#include "curl/multi.h"
#include <future>
#include <queue>

namespace Request {
    class RequestManager final {

    private:
        /// @brief The task for each request
        struct RequestTask {
            RequestParameters parameters; ///< The main parameters for requests
            std::promise<RequestResponse> promise; ///< The promise which occupies the request response
        };

        std::mutex queueMutex; ///< The mutex for each thread to pick data in the queue
        std::queue<RequestTask> tasksQueue; ///< The queue that contains the request tasks
        std::condition_variable tasksCondition; ///< The condition variable for threads to run
        bool stop_pool = false; ///< The boolean that stops the threads

        /// @brief The main worker's loop (for each thread)
        void workerThreadLoop();
    protected:
        /// @brief Constructor of the request manager
        /// @param threadAmounts Amount of threads to run continuously
        RequestManager(unsigned short threadAmounts);
    public:
        /// @brief
        /// @return A singleton of the request manager
        static const RequestManager& getSingleton();

        /// @brief Queues a request to be made
        /// @param requestParameters The request parameters
        /// @return The structure containing the request parameters and promise for result
        std::future<RequestResponse> queueRequestTask(const RequestParameters& requestParameters);
    };
} // Request