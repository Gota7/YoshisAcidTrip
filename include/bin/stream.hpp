#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <vector>

// Either assigns the declaration an optional value or returns false.
#define OPT_ASSIGN(decl, tmp, val) tmp = val; if (!tmp) return false; decl = *tmp

// Swap a 16-bit value.
#define SWAP16(val) ((val & 0xFF) << 8) | ((val & 0xFF00) >> 8)
#define SWAP32(val) (((val & 0xFF) << 24) | ((val & 0xFF00) << 8) | ((val & 0xFF0000) >> 8) | ((val & 0xFF000000) >> 24))

// Stream mode.
enum class BStreamMode
{
    Read,
    Write,
    ReadWrite
};

// A general streaming interface.
class BStream
{
public:

    // Set the position. Returns absolute position. If a writer, will add empty bytes if needed, else will just go back to the beginning.
    virtual std::streampos Seek(std::streamoff off, std::ios::seekdir dir = std::ios::beg) = 0;

    // Get the position.
    virtual std::streampos Tell() = 0;

    // Read data from the stream. Returns if successful. Only supported on some stream types.
    virtual bool Read(void* dest, std::size_t len)
    {
        return false;
    }

    // Write data to a stream. Returns if successful. Only supported on some stream types.
    virtual bool Write(const void* src, std::size_t len)
    {
        return false;
    }

    // Destructor.
    virtual ~BStream() {}

    // Get the stream size.
    std::size_t Size()
    {
        std::streampos bak = Tell();
        Seek(0, std::ios::end);
        std::size_t ret = Tell();
        Seek(bak);
        return ret;
    }

    // Set the position. Returns absolute position.
    std::streampos Seek(const std::streampos& pos)
    {
        return Seek(std::streamoff(pos), std::ios::beg);
    }

    // Align the reader.
    bool Align(unsigned int alignment)
    {
        std::streampos pos;
        while (pos = Tell(), pos % alignment)
        {
            if ((long)Seek(1, std::ios::cur) != (long)pos + 1) return false;
        }
        return true;
    }

    // Read data from the stream. Item must be default-constructible.
    template <typename T>
    std::optional<T> Read()
    {
        T ret;
        if (Read(&ret, sizeof(T))) return std::optional<T>(ret);
        else return std::nullopt;
    }

    // Read data at a position.
    template <typename T>
    std::optional<T> ReadAt(const std::streamoff& off, std::ios::seekdir dir = std::ios::beg)
    {
        std::streampos bak = Tell();
        Seek(off, dir);
        auto ret = Read<T>();
        Seek(bak);
        return ret;
    }

    // Read data at a position.
    template <typename T>
    std::optional<T> ReadAt(const std::streampos& pos)
    {
        std::streampos bak = Tell();
        Seek(pos);
        auto ret = Read<T>();
        Seek(bak);
        return ret;
    }

    // Write data to the stream.
    template <typename T>
    bool Write(const T& data)
    {
        return Write(&data, sizeof(T));
    }

    // Write data at a position.
    template <typename T>
    bool WriteAt(const T& data, const std::streamoff& off, std::ios::seekdir dir = std::ios::beg)
    {
        std::streampos bak = Tell();
        Seek(off, dir);
        auto ret = Write<T>(data);
        Seek(bak);
        return ret;
    }

    // Write data at a position.
    template <typename T>
    bool WriteAt(const T& data, const std::streampos& pos)
    {
        std::streampos bak = Tell();
        Seek(pos);
        auto ret = Write<T>(data);
        Seek(bak);
        return ret;
    }

    // Write a group of 0s. Returns if it succeeds.
    bool Pad(std::size_t len)
    {
        for (std::size_t i = 0; i < len; i++)
        {
            if (!Write<uint8_t>(0)) return false;
        }
        return true;
    }

    // Read a fixed-len string (padded with 0s).
    std::optional<std::string> ReadFixedLenStr(std::size_t len)
    {
        std::string ret = "";
        std::optional<char> c;
        do
        {
            if (len == 0) return ret;
            c = Read<char>();
            if (!c) return std::nullopt;
            len--;
            ret += *c;
        } while (*c != 0);
        return ret;
    }

    // Read a C-style string.
    std::optional<std::string> ReadCStr(std::optional<std::size_t> maxLen = std::nullopt)
    {
        std::size_t len = SIZE_MAX;
        if (maxLen) len = *maxLen;
        return ReadFixedLenStr(len);
    }

    // Write a fixed-len string (padded with 0s).
    bool WriteFixedLenStr(const std::string& str, std::size_t maxLen)
    {
        std::size_t dataLen = glm::max(str.size() + 1, maxLen - str.size() - 1);
        std::size_t paddingLen = maxLen - dataLen;
        if (!Write(str.data(), dataLen)) return false;
        Pad(paddingLen);
    }

    // Write a C-style string.
    bool WriteCStr(const std::string& str)
    {
        return Write(str.data(), str.size() + 1);
    }

    // Write data from another stream to this one.
    bool Copy(BStream& other, std::size_t len)
    {
        std::vector<char> tmp;
        tmp.resize(len);
        if (!other.Read(tmp.data(), len)) return false;
        return Write(tmp.data(), len);
    }

};