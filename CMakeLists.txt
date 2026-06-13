# Important to at least be version 3.11
cmake_minimum_required(VERSION 3.11)
project(Benny)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# {!} START for Configuration
SET(ZSTD_VERSION 1.5.7)
SET(DOWNLOAD_EXTRACT_TIMESTAMP 1)
# {!} END for Configuration

add_library(BennyLib STATIC
        Encodings/ZStandard.cpp
        Encodings/ZStandard.hpp
        Encodings/EncodingsInclude.hpp
)

add_executable(Benny main.cpp)
target_link_libraries(Benny PRIVATE BennyLib)

# {!} START for ZSTD --
set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)
set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
        zstd
        URL "https://github.com/facebook/zstd/releases/download/v${ZSTD_VERSION}/zstd-${ZSTD_VERSION}.tar.gz"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        SOURCE_SUBDIR build/cmake
)

FetchContent_MakeAvailable(zstd)

target_link_libraries(
        BennyLib
        PUBLIC
        libzstd_static
)

# On windows and macos this is needed
target_include_directories(
        BennyLib
        PUBLIC
        ${zstd_SOURCE_DIR}/lib
)
# {!} END for ZSTD ─────────────────────────────────────────────────────────────────

# {!} START for OpenSSL ─────────────────────────────────────────────────────────────────
IF(WIN32)
    SET(OPENSSL_ROOT_DIR "C:/msys64/mingw64") # Important d'installer MINGW32 -> MINGW64
endif()

find_package(OpenSSL REQUIRED)

target_link_libraries(
        BennyLib
        PUBLIC
        OpenSSL::Crypto
        OpenSSL::SSL
)
# {!} END for OpenSSL ─────────────────────────────────────────────────────────────────

# {!} START for libcurl ─────────────────────────────────────────────────────────────────
FetchContent_Declare(
        curl
        URL "https://github.com/curl/curl/releases/download/curl-8_20_0/curl-8.20.0.tar.gz"
)

set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CURL_USE_OPENSSL ON CACHE BOOL "" FORCE)
set(HTTP_ONLY OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(curl)

target_link_libraries(
        BennyLib
        PUBLIC
        libcurl
)
# {!} END for libcurl ─────────────────────────────────────────────────────────────────

# {!} START for GTest ─────────────────────────────────────────────────────────────────
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(tests Tests/ZStandard.cpp
        Tests/CUrl.cpp
        Tests/OpenSSL.cpp
)
target_link_libraries(tests PRIVATE GTest::gtest_main BennyLib)

include(GoogleTest)
gtest_discover_tests(tests PROPERTIES PROCESSORS 4)
# {!} END for GTest ─────────────────────────────────────────────────────────────────

# {!} START for file transferring ─────────────────────────────────────────────────────────────────
configure_file(${CMAKE_SOURCE_DIR}/cacert.pem ${CMAKE_BINARY_DIR}/cacert.pem COPYONLY)
# {!} END for file transferring ─────────────────────────────────────────────────────────────────