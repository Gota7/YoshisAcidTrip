#pragma once

#include "../stream.hpp"

// Standard Nintendo compression format.
namespace Yaz0
{

    // Decompress from a stream. If no max length of the compression is given, it will use the entire stream.
    bool Decompress(BStream& src, BStream& dst, std::optional<std::size_t> maxSize = std::nullopt);

    // Compress a file of a given size from the source to the destination.
    bool Compress(BStream& src, BStream& dst, std::size_t fileSize, unsigned int dataAlignment = 0, unsigned char level = 7);

};