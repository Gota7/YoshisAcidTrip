#pragma once

#include <bin/types.hpp>
#include <cstdint>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <vector>

// Either assigns the declaration an optional value or returns false.
#define OPT_ASSIGN(decl, tmp, val) tmp = val; if (!tmp) return false; decl = *tmp

// Swap values.
#define SWAP16(val) ((val & 0xFF) << 8) | ((val & 0xFF00) >> 8)
#define SWAP32(val) (((val & 0xFF) << 24) | ((val & 0xFF00) << 8) | ((val & 0xFF0000) >> 8) | ((val & 0xFF000000) >> 24))
#define SWAP64(val) (((val & 0xFF) << 56) | ((val & 0xFF00) << 40) | ((val & 0xFF0000) << 24) | ((val & 0xFF000000) << 8) | ((val & 0xFF00000000) >> 8) | ((val & 0xFF0000000000) >> 24) | ((val & 0xFF000000000000) >> 40) | ((val & 0xFF00000000000000) >> 56))

// Helpers. Copied from OEAD.
template <typename, template <typename> class, typename = std::void_t<>>
struct Detect : std::false_type {};
template <typename T, template <typename> class Op>
struct Detect<T, Op, std::void_t<Op<T>>> : std::true_type {};
template <typename T>
using ExposesFieldsImpl = decltype(std::declval<T>().fields());
template <typename T>
using ExposesFields = Detect<T, ExposesFieldsImpl>;
#define DEFINE_FIELDS(TYPE, ...)                                                                    \
    constexpr auto fields() { return std::tie(__VA_ARGS__); }                                       \
    constexpr auto fields() const { return std::tie(__VA_ARGS__); }                                 \
    constexpr friend bool operator==(const TYPE& lhs, const TYPE& rhs) {                            \
        return lhs.fields() == rhs.fields();                                                        \
    }                                                                                               \
    constexpr friend bool operator!=(const TYPE& lhs, const TYPE& rhs) { return !(lhs == rhs); }    \
    template <typename H>                                                                           \
    friend H AbslHashValue(H h, const TYPE& self) {                                                 \
        return H::combine(std::move(h), self.fields());                                             \
}

// Stream mode.
enum class BStreamMode
{
    Read,
    Write,
    ReadWrite
};

// Endian.
enum class BStreamEndian
{
    Native,
    Little,
    Big
};

// Swap utility.
namespace BSwapUtil
{

    // Swap a value.
    template <int count>
    inline void SwapValue(void* val);
    template <>
    inline void SwapValue<1>(void* val) {}
    template <>
    inline void SwapValue<2>(void* val)
    {
        u16 tmp = *(u16*)val;
        *(u16*)val = SWAP16(tmp);
    }
    template <>
    inline void SwapValue<4>(void* val)
    {
        u32 tmp = *(u32*)val;
        *(u32*)val = SWAP32(tmp);
    }
    template <>
    inline void SwapValue<8>(void* val)
    {
        u64 tmp = *(u64*)val;
        *(u64*)val = SWAP64(tmp);
    }

    // Swap a value's endian.
    template <typename T>
    inline T SwapValueEndian(T data)
    {
        static_assert(std::is_arithmetic<T>());
        SwapValue<sizeof(data)>(&data);
        return data;
    }

}

// A general streaming interface.
class BStream
{
public:
    BStreamEndian endian = BStreamEndian::Native;

    // Set the position. Returns absolute position. If a writer, will add empty bytes if needed, else will just go back to the beginning.
    virtual std::streampos Seek(std::streamoff off, std::ios::seekdir dir = std::ios::beg) = 0;

    // Get the position.
    virtual std::streampos Tell() = 0;

    // Read data from the stream. Returns if successful. Only supported on some stream types. This will NOT swap endian!
    virtual bool Read(void* dest, std::size_t len)
    {
        return false;
    }

    // Write data to a stream. Returns if successful. Only supported on some stream types. This will NOT swap endian!
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
        if (Read(&ret, sizeof(T)))
        {
            SwapEndianInPlace(ret, endian);
            return std::optional<T>(ret);
        }
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
        T swapped = SwapEndian(data);
        return Write(&swapped, sizeof(T));
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
        while (len)
        {
            c = Read<char>();
            if (!c) return std::nullopt;
            len--;
            ret += *c;
        }
        return ret;
    }

    // Read a C-style string.
    std::optional<std::string> ReadCStr(std::optional<std::size_t> maxLen = std::nullopt)
    {
        std::size_t len = SIZE_MAX;
        if (maxLen) len = *maxLen;
        std::string ret = "";
        std::optional<char> c;
        do
        {
            if (len == 0) return ret;
            c = Read<char>();
            if (!c) return std::nullopt;
            len--;
            if (*c != 0) ret += *c;
        } while (*c != 0);
        return ret;
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

    // For getting the system endian. Never will return Native, but what Native is.
    static BStreamEndian SystemEndian()
    {
        return BStreamEndian::Little;
    }

    // For swapping the endian of an item if needed.
    template <typename T>
    static void SwapEndianInPlace(T& item, BStreamEndian endian)
    {
        if (endian == BStreamEndian::Native || SystemEndian() == endian) return;
        if constexpr (std::is_arithmetic<T>())
            item = BSwapUtil::SwapValueEndian(item);
        if constexpr (ExposesFields<T>())
            std::apply([endian](auto&... fields) { (SwapEndianInPlace(fields, endian), ...); }, item.fields());
    }

    // For swapping endian if needed.
    template <typename T>
    T SwapEndian(T item)
    {
        SwapEndianInPlace(item, endian);
        return item;
    }

};