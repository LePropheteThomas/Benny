#pragma once

#include "zstd.h"
#include <vector>

namespace Encodings
{
    class ZStandardStreaming
    {
    private:
        ZSTD_CCtx* cctx = nullptr; ///< The streaming compression context for ZSTD's lifetime
        ZSTD_DCtx* dctx = nullptr; ///< The streaming decompression context for ZSTD's lifetime

    protected:
        /// @brief Default constructor
        /// @param compressionLevel The level of compression for the given stream context
        /// @param nmbOfWorkers The amount of workers for a given stream context
        ZStandardStreaming(int compressionLevel = 3, int nmbOfWorkers = 1);

        /// @brief Default destructor
        ~ZStandardStreaming();

    public:
        // Makes it into a singleton
        ZStandardStreaming(const ZStandardStreaming&) = delete;
        ZStandardStreaming& operator=(const ZStandardStreaming&) = delete;

        /// @brief Decompresses data using the current decompression context
        /// @param compressedData The compressed data
        /// @return The decompressed data
        [[nodiscard]] std::vector<char> decompressData(const std::vector<char>& compressedData) const;

        /// @brief Decompresses data using the current compression context
        /// @param decompressedData The raw/decompressed data
        /// @return The compressed data
        [[nodiscard]] std::vector<char> compressData(const std::vector<char>& decompressedData) const;

        /// @brief Returns an initialized ZStandardStreaming singleton
        /// @return A reference to an initialized ZStandardStreaming singleton
        [[nodiscard]] static const ZStandardStreaming& getStream();
    };
}
