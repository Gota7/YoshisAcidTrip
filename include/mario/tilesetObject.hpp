#pragma once

#include <bin/stream.hpp>
#include <common.hpp>

// Tileset object.
class MTilesetObject
{
    std::vector<std::vector<u8>> tiles; // Indexed by row, column.
    std::size_t subPartRow;
    bool reversed = false;

public:

    // Create an object from a stream.
    MTilesetObject(BStream& src);

};