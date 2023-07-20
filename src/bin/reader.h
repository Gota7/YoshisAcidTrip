#pragma once

#include "endian.h"
#include "source.h"
#include <common.hpp>
#include <iostream>
#include <string>
#include <types.h>

// Binary reader. Each has its own position even for shared streams.
struct BReader
{
    std::shared_ptr<BSource> src; // Source to read from.
    std::streampos currPos = 0; // Current position. Do not write to this.
    bool swapEndian = false;

    // Create a new reader.
    BReader(std::shared_ptr<BSource> src) : src(src) {}

    // Copy from another reader.
    BReader(BReader&& other) : src(other.src), currPos(other.currPos), swapEndian(other.swapEndian) {}

    // Copy from another reader.
    BReader(const BReader& other) : src(other.src), currPos(other.currPos), swapEndian(other.swapEndian) {}

    // Set endian.
    void SetEndian(BEndian endian)
    {
        union {
            uint32_t i;
            char c[4];
        } bint = {0x01020304};
        bool systemBig = bint.c[0] == 1;
        switch (endian)
        {
            case BEndian::Native: swapEndian = false; break;
            case BEndian::Little: swapEndian = systemBig; break;
            case BEndian::Big: swapEndian = !systemBig; break;
        }
    }

    // Get the next character.
    u8 U8()
    {
        src->data->seekg(currPos);
        char ret;
        currPos = src->data->read(&ret, sizeof(u8)).tellg();
        return (u8)ret;
    }

    // Get the next unsigned short.
    u16 U16()
    {
        src->data->seekg(currPos);
        u16 ret;
        currPos = src->data->read((char*)&ret, sizeof(u16)).tellg();
        if (swapEndian) ret = BSwap16(ret);
        return ret;
    }

    // Get the next unsigned int.
    u32 U32()
    {
        src->data->seekg(currPos);
        u32 ret;
        currPos = src->data->read((char*)&ret, sizeof(u32)).tellg();
        if (swapEndian) ret = BSwap32(ret);
        return ret;
    }

    // Read chars.
    std::string Chars(std::size_t size)
    {
        std::string ret = "";
        for (std::size_t i = 0; i < size; i++)
        {
            ret += (char)U8();
        }
        return ret;
    }

    // Get position.
    std::streampos Tell()
    {
        return currPos;
    }

    // Set position.
    void Set(std::streampos pos)
    {
        currPos = pos;
    }

    // Advance reader by an offset.
    void Advance(std::streamoff off)
    {
        currPos = src->data->seekg(currPos).seekg(off, std::ios::cur).tellg();
    }

};