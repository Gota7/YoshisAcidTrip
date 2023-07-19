#pragma once

#include <jsystem/fs.hpp>
#include <jsystem/mem.hpp>
#include <functional>
#include <memory>
#include <string>

// A texture format.
class TFormat
{
public:

    // Get texture properties.
    virtual void Properties(int& width, int& height, int& numChannels) = 0;

    // Get a pointer to the data. Assume it is in R8G8B8A8 format.
    virtual const void* DataPtr() = 0;

    // Free the texture.
    virtual ~TFormat() {}

};

// Instantiate a texture.
using TTextureLoader = std::function<JPtr<TFormat>(const JResPath&)>;