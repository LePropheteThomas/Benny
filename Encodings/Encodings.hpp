#pragma once

#include <vector>

namespace Encodings {
    class Encodings {
    protected:
        /// @brief Default constructor
        Encodings() = default;
    public:
        /// @brief Default destructor
        virtual ~Encodings() = default;

        /// @brief Compresses data using the current compression context
        /// @param decompressedData The raw/decompressed data
        /// @return The compressed data
        [[nodiscard]] virtual std::vector<char> compressData(const std::vector<char>& decompressedData) const = 0;

        /// @brief Decompresses data using the current decompression context
        /// @param compressedData The compressed data
        /// @return The decompressed data
        [[nodiscard]] virtual std::vector<char> decompressData(const std::vector<char>& compressedData) const = 0;
    };
}