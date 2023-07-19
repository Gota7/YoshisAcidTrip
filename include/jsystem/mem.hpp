#pragma once

#include "constants.h"
#include <memory>

// Memory allocation functions, can be modified to use from custom heap. Also is a lot less typing, seriously why is it SO LONG to type out one of the best C++ features.

// Standard allocation, avoid if possible.
#define JAlloc(val, type, args...) val = new type(args)

// Standard array allocation, avoid if possible.
#define JAllocArr(val, type, count) val = new type[count]

// Standard deallocation, avoid if possible.
#define JFree(val) delete val

// Unique pointer type.
#define JPtr std::unique_ptr

// Shared pointer type.
#define JSharedAsset std::shared_ptr

// Expirable shared type.
#define JSharedAssetExpirable std::weak_ptr

// Make a unique pointer.
#define JPtrMake(type, args...) std::make_unique<type>(args)

// Make a unique pointer to an array.
#define JPtrMakeArr(type, count) std::make_unique<type[]>(count)

// Create a shared asset.
#define JSharedAssetCreate(type, args...) std::make_shared<type>(args)