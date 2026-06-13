#pragma once

#include "zstd.h"
#include <vector>

#include "Encodings.hpp"

namespace Encodings
{
    class ZStandardStreaming : public Encodings
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

        [[nodiscard]] std::vector<char> decompressData(const std::vector<char>& compressedData) const override;

        [[nodiscard]] std::vector<char> compressData(const std::vector<char>& decompressedData) const override;

        [[nodiscard]] static const ZStandardStreaming& getSingleton();
    };
}
