#include "ZStandard.hpp"

namespace Encodings
{
    ZStandardStreaming::ZStandardStreaming(int compressionLevel, int nmbOfWorkers) : Encodings()
    {
        this->cctx = ZSTD_createCCtx();
        this->dctx = ZSTD_createDCtx();

        ZSTD_CCtx_setParameter(this->cctx, ZSTD_cParameter::ZSTD_c_compressionLevel, compressionLevel);
        ZSTD_CCtx_setParameter(this->cctx, ZSTD_cParameter::ZSTD_c_nbWorkers, nmbOfWorkers);
    }

    ZStandardStreaming::~ZStandardStreaming()
    {
        ZSTD_freeCCtx(this->cctx);
        ZSTD_freeDCtx(this->dctx);
    }

    std::vector<char> ZStandardStreaming::decompressData(const std::vector<char>& compressedData) const
    {
        ZSTD_inBuffer compressedBufferData(compressedData.data(), compressedData.size(), 0);

        // Separates staging from accumulator to prevent 100000< zeros
        size_t decompressedDataTemporaryBufferRecommendedSize = ZSTD_DStreamOutSize();
        std::vector<char> stagingBuffer(decompressedDataTemporaryBufferRecommendedSize);
        std::vector<char> decompressedData;

        // Look while there still is data
        while (compressedBufferData.pos < compressedBufferData.size)
        {
            ZSTD_outBuffer temporaryDecompressedDataBuffer(stagingBuffer.data(), stagingBuffer.size(), 0);
            size_t const returnedDecompressedSize = ZSTD_decompressStream(this->dctx, &temporaryDecompressedDataBuffer, &compressedBufferData);

            if (ZSTD_isError(returnedDecompressedSize))
                break;

            if (temporaryDecompressedDataBuffer.pos)
            {
                const char* convertedDecompressedTemporaryData = static_cast<const char*>(temporaryDecompressedDataBuffer.dst);
                decompressedData.insert(decompressedData.end(), convertedDecompressedTemporaryData, convertedDecompressedTemporaryData + temporaryDecompressedDataBuffer.pos);
            }
        }

        return decompressedData;
    }

    std::vector<char> ZStandardStreaming::compressData(const std::vector<char>& decompressedData) const
    {
        ZSTD_inBuffer rawBufferData(decompressedData.data(), decompressedData.size(), 0);

        size_t compressedDataTemporaryBufferRecommendedSize = ZSTD_CStreamOutSize();
        std::vector<char> compressedDataBuffer(compressedDataTemporaryBufferRecommendedSize);

        std::vector<char> compressedData;

        // Process data while there still is
        while (rawBufferData.pos < rawBufferData.size)
        {
            size_t remaining;
            do
            {
                // Check for data and gives appropriate directive to compression stream
                ZSTD_EndDirective const mode = (rawBufferData.pos >= rawBufferData.size)
                    ? ZSTD_e_end
                    : ZSTD_e_continue;

                ZSTD_outBuffer temporaryCompressedDataBuffer(compressedDataBuffer.data(), compressedDataBuffer.size(), 0);
                remaining = ZSTD_compressStream2(this->cctx, &temporaryCompressedDataBuffer, &rawBufferData, mode);

                if (ZSTD_isError(remaining))
                    break;

                // If there is data, put it inside the staging and then in the accumulator
                if (temporaryCompressedDataBuffer.pos)
                {
                    const char* convertedCompressedTemporaryData = static_cast<const char*>(temporaryCompressedDataBuffer.dst);
                    compressedData.insert(compressedData.end(), convertedCompressedTemporaryData, convertedCompressedTemporaryData + temporaryCompressedDataBuffer.pos);
                }
            } while (remaining != 0);
        }

        return compressedData;
    }

    const ZStandardStreaming & ZStandardStreaming::getSingleton() {
        static ZStandardStreaming inst;
        return inst;
    }
}
