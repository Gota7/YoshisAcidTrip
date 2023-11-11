#pragma once

#include "../bin/streams/memory.hpp"
#include "zip/zip.h"

// Read a ZIP file.
class PZIP
{
    zip_t* zip;
public:

    // Create a ZIP file from a file path.
    PZIP(const std::string& path);

    // Open a file. Returns nothing if non-existent.
    std::optional<BStreamMemory> Open(const std::string& path);

    // Read as a string. Returns nothing if non-existent.
    std::optional<std::string> String(const std::string& path);

    // Get a file size. Returns -1 if invalid.
    ssize_t FileSize(const std::string& path);

    // Read a file into a buffer directly. Returns how much is read.
    ssize_t Read(const std::string& path, void* buf);

    // Get a list of files.
    std::vector<std::string> FileList();

    // Destructor.
    ~PZIP();

};