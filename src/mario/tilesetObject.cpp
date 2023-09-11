#include <mario/tilesetObject.hpp>

MTilesetObject::MTilesetObject(BStream& src)
{
    ZoneScopedN("MTilesetObject::MTilesetObject");

    // Reverse check.
    u8 currByte = *src.Read<u8>();
    reversed = currByte & 0x82;
    std::vector<u8> currRow;

    // Read object.
    while (true)
    {

        // Object over.
        if (currByte == 0xFF) return;

        // Row over.
        else if (currByte == 0xFE)
        {
            tiles.push_back(currRow);
            currRow.clear();
        }

        // Sub part.
        else if (currByte & 0x80)
        {
            subPartRow = tiles.size();
            currRow.push_back(currByte);
        }

        // Standard part.
        else
        {
            std::optional<u8> nextTmp = src.Read<u8>();
            u8 next = nextTmp ? (*nextTmp) : 0;
            nextTmp = src.Read<u8>();
            u8 extra = nextTmp ? (*nextTmp) : 0;
            currRow.push_back(currByte);
            currRow.push_back((u8)(next | ((extra & 3) << 8)));
            currRow.push_back((u8)(extra >> 2));
            currByte = *src.Read<u8>();
        }

        // Next byte.
        auto tmp = src.Read<u8>();
        if (!tmp)
        {
            if (currRow.size() > 0) tiles.push_back(currRow);
            return;
        }
        currByte = *tmp;

    }

}