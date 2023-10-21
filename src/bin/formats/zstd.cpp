#include "zstd.hpp"

#include "../../common.hpp"
#include "../../jsystem/mem.hpp"
#include "../types.hpp"
#include <zstd.h>

bool ZSTD::Decompress(BStream& src, BStream& dst, std::size_t srcSize)
{
    ZoneScopedN("ZSTD::Decompress");

    // Idk how to make this compatible with BStreams! Evil evil extra heap allocations but we should only be calling these on file loads which should be rare enough.
    JPtr<u8[]> srcData = JPtrMake(u8[], srcSize);

    // Read source stream.
    if (!src.Read(srcData.get(), srcSize))
    {
        DBG_PRINT("ZSTD@LSD: Requested source file to decompress size bigger than stream!");
        return false;
    }

    // Prepare destination.
    std::size_t dstSize = ZSTD_getFrameContentSize(srcData.get(), srcSize);
    if (!dstSize)
    {
        DBG_PRINT("ZSTD@LSD: Unknown decompressed destination stream size!");
        return false;
    }
    JPtr<u8[]> dstData = JPtrMake(u8[], dstSize);
    
    // Get destination and write.
    ZSTD_decompress(dstData.get(), dstSize, srcData.get(), srcSize);
    return dst.Write(dstData.get(), dstSize);

}

bool ZSTD::Compress(BStream& src, BStream& dst, std::size_t srcSize, unsigned char level)
{
    ZoneScopedN("ZSTD::Compress");

    // Once again, idk how to make this compatible with BStreams! Yeah yeah evil extra heap allocs...
    JPtr<u8[]> srcData = JPtrMake(u8[], srcSize);

    // Read source stream.
    if (!src.Read(srcData.get(), srcSize))
    {
        DBG_PRINT("ZSTD@LSD: Requested source file to compress size bigger than stream!");
        return false;
    }

    // Prepare destination.
    std::size_t dstSize = ZSTD_compressBound(srcSize);
    if (!dstSize)
    {
        DBG_PRINT("ZSTD@LSD: Unknown compressed destination stream size!");
        return false;
    }
    JPtr<u8[]> dstData = JPtrMake(u8[], dstSize);

    // Do decompression. Note the "bound" from earlier is a max so it can be smaller.
    dstSize = ZSTD_compress(dstData.get(), dstSize, srcData.get(), srcSize, level);
    return dst.Write(dstData.get(), dstSize);

}