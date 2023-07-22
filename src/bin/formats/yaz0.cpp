#include <bin/formats/yaz0.hpp>

#include <bin/types.hpp>
#include <common.hpp>

bool Yaz0::Decompress(BStream& src, BStream& dst, std::optional<std::size_t> maxSize)
{
    std::size_t len = maxSize ? *maxSize : (src.Size() - src.Tell()); // May not use all, is just upper bound.

    // Read header.
    if (len <= 0x10) return false;
    len -= 0x10;
    auto maybeMagic = src.ReadFixedLenStr(4);
    if (!maybeMagic || *maybeMagic != std::string("Yaz0")) return false; // Also checks for readability.
    u32 tmp = *src.Read<u32>();
    int uncompressedSize = (int)SWAP32(tmp);
    *src.Read<u32>(); // Data alignment.
    src.Read<u32>(); // Reserved.

    // Need temporary buffer for self-referencing chunks.
    std::vector<char> tmpBuf;
    tmpBuf.resize(uncompressedSize);

    // Read data until done.
    int chunks = 0;
    u8 header = 0;
    while (true)
    {

        // Reset header if needed.
        if (chunks == 0)
        {
            if (len-- < 1) return false;
            header = *src.Read<u8>();
        }

        // Read bytes.
        if (header & 0x80)
        {
            if (len-- < 1) return false;
            tmpBuf.push_back(*src.Read<u8>());
            if (--uncompressedSize <= 0) break;
        }
        else
        {

            // Get section from before to copy.
            if (len < 3) return false;
            len -= 3;
            u16 pair = *src.Read<u16>();
            u16 dist = (pair & 0x0FFF) + 1;
            u16 length = ((pair >> 12) ? (pair >> 12) : (*src.Read<u8>() + 16)) + 2;

            // Copy section.
            char* base = tmpBuf.data() + tmpBuf.size() - dist;
            if (base < tmpBuf.data() || base + length > tmpBuf.data() + tmpBuf.size()) return false;
            for (std::size_t i = 0; i < length; i++)
            {
                tmpBuf.push_back(base[i]);
            }
            uncompressedSize -= length;
            if (uncompressedSize <= 0) break;
            
        }

        // Shift groups.
        header <<= 1;
        chunks--;

    }

    // Copy to destination.
    dst.Write(tmpBuf.data(), tmpBuf.size());
    return true;

}