#include "RequestManager.hpp"

namespace Request {
    void RequestManager::workerThreadLoop() {
        CURL* curlHandle = curl_easy_init();
        std::unique_ptr<RequestTask> task = nullptr;

        if (!curlHandle) // Could not initiate properly
            return;

        while (true) {
            { // Locks the mutex to allow only one thread to run per cycle of len(tasksQueue).
                std::unique_lock<std::mutex> lock(this->queueMutex);

                this->tasksCondition.wait(lock, [this] {
                    return !this->tasksQueue.empty() || this->stop_pool;
                });

                // Puts an end to the thread if the brake has been triggered (stop_pool)
                if (this->stop_pool && this->tasksQueue.empty())
                    break;

                task = std::make_unique<RequestTask>(std::move(this->tasksQueue.front()));
                this->tasksQueue.pop();
            }

            // TODO: Finish the requestManager::workerThreadLoop for request
        }

        curl_easy_cleanup(curlHandle);
    }

    RequestManager::RequestManager(unsigned short threadAmounts) {
        workerThreads.reserve(threadAmounts);
        for (unsigned short i = 0; i < threadAmounts; i++)
            workerThreads.emplace_back(&RequestManager::workerThreadLoop, this);
    }

    RequestManager::~RequestManager() {
        { // Obtains the lock and sets stop_pool at true, which let's wait for lost wakeup
            std::lock_guard<std::mutex> lock(this->queueMutex);
            this->stop_pool = true;
        }

        tasksCondition.notify_all();

        // Joins the threads so that the main thread won't exit until all the child
        // threads aren't killed
        for (std::thread& task : workerThreads)
            if (task.joinable())
                task.join();
    }

    const RequestManager& RequestManager::getSingleton() {
        static const RequestManager manager(5);
        return manager;
    }

    std::future<RequestResponse> RequestManager::queueRequestTask(const RequestParameters &requestParameters) {
        std::promise<RequestResponse> promise;
        std::future<RequestResponse> future = promise.get_future();

        {
            std::lock_guard<std::mutex> lock(this->queueMutex);
            this->tasksQueue.push(RequestTask(requestParameters, std::move(promise)));
        }

        this->tasksCondition.notify_one();
        return future;
    }
}
