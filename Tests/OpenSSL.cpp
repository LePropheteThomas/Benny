#include "openssl/ssl.h"
#include "openssl/crypto.h"
#include "gtest/gtest.h"

/// @brief Functional test
TEST(OpenSSL, OpenSSLFunctionalLinking)
{
    EXPECT_NE(OpenSSL_version(OPENSSL_VERSION), nullptr) << "Functional linking failed!";
}