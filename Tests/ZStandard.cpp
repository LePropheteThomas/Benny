#include "gtest/gtest.h"
#include "../Encodings/Encodings.hpp"

/// @brief Compression and decompression roundtrip test (small string)
TEST(ZStandard, SmallCompressionDecompressionRoundtrip)
{
    // Setup
    const Encodings::ZStandardStreaming& stream = Encodings::ZStandardStreaming::getStream();
    const std::string original = "Hello, this is a test string for ZSTD compression and decompression!";
    const std::vector<char> originalVector(original.begin(), original.end());

    std::vector<char> compressedVector = stream.compressData(originalVector);
    std::vector<char> decompressedVector = stream.decompressData(compressedVector);
    EXPECT_EQ(decompressedVector, originalVector) << "Roundtrip compression/decompression failed!";
}

/// @brief Compression and decompression roundtrip test (large string)
TEST(ZStandard, LargeCompressionDecompressionRoundtrip)
{
    const Encodings::ZStandardStreaming& stream = Encodings::ZStandardStreaming::getStream();
    const std::string original(10000000, 'a');
    const std::vector<char> originalVector(original.begin(), original.end());

    std::vector<char> compressedVector = stream.compressData(originalVector);
    std::vector<char> decompressedVector = stream.decompressData(compressedVector);
    EXPECT_EQ(decompressedVector, originalVector) << "Large roundtrip compression/decompression failed!";
}

/// @brief Functional test
TEST(ZStandard, FunctionalLinking)
{
    const std::string input = "hello";
    const size_t compressBound = ZSTD_compressBound(input.size());

    std::vector<char> compressed(compressBound);
    const size_t compressedSize = ZSTD_compress(compressed.data(), compressBound, input.data(), input.size(), 1);
    ASSERT_FALSE(ZSTD_isError(compressedSize)) << ZSTD_getErrorName(compressedSize);
    compressed.resize(compressedSize);

    std::vector<char> decompressed(input.size());
    const size_t decompressedSize = ZSTD_decompress(decompressed.data(), input.size(), compressed.data(), compressedSize);
    ASSERT_FALSE(ZSTD_isError(decompressedSize)) << ZSTD_getErrorName(decompressedSize);

    ASSERT_EQ(input, std::string(decompressed.begin(), decompressed.end())) << "Functional linking failed!";
}