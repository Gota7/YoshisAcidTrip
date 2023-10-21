#include "yaz0.hpp"

#include "../../common.hpp"
#include "../types.hpp"

bool Yaz0::Decompress(BStream& src, BStream& dst, std::optional<std::size_t> maxSize)
{
    ZoneScopedN("Yaz0::Decompress");
    std::size_t len = maxSize ? *maxSize : (src.Size() - src.Tell()); // May not use all, is just upper bound.

    // Read header.
    if (len <= 0x10) return false;
    len -= 0x10;
    auto maybeMagic = src.ReadFixedLenStr(4);
    if (!maybeMagic || *maybeMagic != std::string("Yaz0")) return false; // Also checks for readability.
    BStreamEndian bakEndian = src.endian;
    src.endian = BStreamEndian::Big;
    int uncompressedSize = *src.Read<u32>();
    src.Read<u32>(); // Data alignment.
    src.Read<u32>(); // Reserved.
    src.endian = bakEndian;

    // Need temporary buffer for self-referencing chunks as seeking repeatedly is not efficient.
    std::vector<char> tmpBuf;
    tmpBuf.reserve(uncompressedSize);
    char* tmpBufEnd = tmpBuf.data() + uncompressedSize;

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
            chunks = 8;
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
            if (len < 2) return false;
            len -= 2;
            u8 b1 = *src.Read<u8>();
            u8 b2 = *src.Read<u8>();
            char* base = tmpBuf.data() + tmpBuf.size() - (( b1 & 0x0f ) << 8 | b2 ) - 1;
            int length = b1 >> 4;

            // Calculate length.
            if (!length)
            {
                if (len-- < 1) return false;
                length = *src.Read<u8>() + 0x12;
            }
            else
            {
                length += 2;
            }

            // Copy section.
            if (base < tmpBuf.data() || base + length > tmpBufEnd) return false;
            for (int i = 0; i < length; i++)
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