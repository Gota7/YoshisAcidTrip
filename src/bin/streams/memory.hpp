#pragma once

#include "../stream.hpp"
#include <cstring>
#include <vector>

// For reading or writing data in memory.
class BStreamMemory : public BStream
{
    BStreamMode mode;
    std::vector<char> data;
    std::size_t pos = 0;

public:

    // Create a new memory stream. Sets position at 0.
    BStreamMemory(BStreamMode mode, const std::vector<char> data = {}) : mode(mode), data(data) {}

    // Create a new memory stream from another stream.
    BStreamMemory(BStream& other) : mode(BStreamMode::ReadWrite)
    {
        std::streampos bak = other.Tell();
        other.Seek(0);
        Copy(other, other.Size());
        other.Seek(bak);
        Seek(0);
    }

    // Get the data.
    std::vector<char> GetData()
    {
        return data;
    }

    virtual std::streampos Seek(std::streamoff off, std::ios::seekdir dir = std::ios::beg) override
    {
        if (dir == std::ios::cur)
        {
            pos += off;
        }
        else if (dir == std::ios::end)
        {
            pos = data.size() - off;
        }
        else
        {
            pos = off;
        }
        return pos;
    }

    virtual std::streampos Tell() override
    {
        return std::streampos(pos);
    }

    virtual bool Read(void* dest, std::size_t len) override
    {
        if (mode == BStreamMode::Write) return false;
        if (pos + len > data.size()) return false;
        memcpy(dest, data.data() + pos, len);
        pos += len;
        return true;
    }

    virtual bool Write(const void* src, std::size_t len) override
    {
        if (mode == BStreamMode::Read) return false;
        if (pos + len > data.size()) data.resize(pos + len);
        memcpy(data.data() + pos, src, len);
        pos += len;
        return true;
    }

};