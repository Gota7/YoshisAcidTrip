#pragma once

#include "../jsystem/mem.hpp"
#include "stream.hpp"
#include <string>
#include <vector>

// Archive for containing other files.
class BArchive
{
public:

    // Open a file in the archive. Creates a file if it does not exist.
    virtual BStream& Open(const std::string& path, BStreamMode mode) = 0;

    // Rename a single file. Does not work for directories.
    virtual bool Rename(const std::string& oldPath, const std::string& newPath) = 0;

    // Delete a single file. Does not work for directories.
    virtual bool Delete(const std::string& path) = 0;

    // If a file exists.
    virtual bool FileExists(const std::string& path) = 0;

    // Number of files.
    virtual std::size_t NumFiles() = 0;

    // Get files in archive.
    virtual std::vector<std::string> GetFilenames() = 0;

};