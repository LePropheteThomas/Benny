#include "curl/curl.h"
#include "gtest/gtest.h"

/// @brief Functional test
TEST(CUrl, CurlFunctionalLinking)
{
    CURL* curl = curl_easy_init();
    ASSERT_NE(curl, nullptr) << "Functional linking failed!";
    curl_easy_cleanup(curl);
}