#pragma once

#include <constants.h>
#include <types.h>

// Seek value.
enum class BStreamSeek
{
    Absolute,
    Relative
};

// Binary stream.
struct BStream
{

    // Get void pointer to data at current position.
    virtual void* DataPtrRaw() = 0;

    // Get pointer to data at current position.
    template <typename T>
    T* DataPtr()
    {
        return (T*)DataPtrRaw();
    }

    // Get reference to data at current position.
    template <typename T>
    T& DataRef()
    {
        return *DataPtr<T>();
    }

    // Length of the stream.
    virtual std::size_t Length() = 0;

    // Set the position. Returns new position.
    virtual std::size_t Seek(long off, BStreamSeek seek = BStreamSeek::Absolute) = 0;

    // Get the position.
    virtual std::size_t Tell() = 0;

    // Stream gets closed.
    virtual ~BStream() = 0;

};