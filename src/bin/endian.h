#pragma once

#include <types.h>

inline uint16_t BSwap16(uint16_t value)
{
    return (((value & 0x00FF) << 8) |
            ((value & 0xFF00) >> 8));
}
    
inline uint32_t BSwap32(uint32_t value) 
{
    return (((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) <<  8) |
            ((value & 0x00FF0000) >>  8) |
            ((value & 0xFF000000) >> 24));
}

// Endian type.
enum class BEndian
{
    Native,
    Little,
    Big
};