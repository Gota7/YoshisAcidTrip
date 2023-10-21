#pragma once

#include "../stream.hpp"

// Used for compressing files.
namespace ZSTD
{

    // Decompress from a stream. Size is the size of the input stream.
    bool Decompress(BStream& src, BStream& dst, std::size_t srcSize);

    // Compress a file of a given size from the source to the destination.
    bool Compress(BStream& src, BStream& dst, std::size_t srcSize, unsigned char level = 7);

}