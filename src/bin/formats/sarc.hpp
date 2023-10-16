#pragma once

#include "../archive.hpp"
#include "../streams/memory.hpp"
#include <map>

// Sead archive.
class SARC : public BArchive
{
    std::map<std::string, BStreamMemory> files; // Would be able to just pass a view of the stream used to make this archive if reading only, but writing is a thing that exists too so copy to memory.
public:

    // Make an empty archive.
    SARC() {}

    // Read a sead archive from a stream.
    SARC(BStream& src);

    // Write archive to a stream.
    void Write(BStream& dst);

    // V-functions.
    virtual BStream& Open(const std::string& path, BStreamMode mode) override;
    virtual bool Rename(const std::string& oldPath, const std::string& newPath) override;
    virtual bool Delete(const std::string& path) override;
    virtual bool FileExists(const std::string& path) override;
    virtual std::size_t NumFiles() override;
    virtual std::vector<std::string> GetFilenames() override;

};