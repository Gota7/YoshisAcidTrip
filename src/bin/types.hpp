#pragma once

#include <cstdint>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;
using f32 = float;
using f64 = double;

struct u24
{
    u8 data[3]; // Stored in BE order.

    u32 Get() { return (data[0] << 16) | (data[1] << 8) | data[2]; }
    void Set(u32 val)
    {
        data[0] = (val & 0xFF0000) >> 16;
        data[1] = (val & 0xFF00) >> 8;
        data[2] = val & 0xFF;
    }

};