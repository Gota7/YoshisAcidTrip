#pragma once

#include <bin/stream.hpp>
#include <jsystem/fs.hpp>

// For reading or writing file I/O.
class BStreamFile : public BStream
{
    JResPath path;
    BStreamMode mode;
    FILE* file;

public:

    // Create a new file stream.
    BStreamFile(const JResPath& path, BStreamMode mode) : path(path), mode(mode)
    {
        if (!JFileSystem::FileExists(path)) JFileSystem::WriteAllBytes(path, {});
        if (mode == BStreamMode::Read) file = fopen(path.fullPath.c_str(), "r");
        else if (mode == BStreamMode::Write) file = fopen(path.fullPath.c_str(), "w");
        else file = fopen(path.fullPath.c_str(), "r+");
        fseek(file, 0, SEEK_SET);
    }

    // Get the file path.
    JResPath FilePath() const
    {
        return path;
    }

    // Close file automatically.
    ~BStreamFile()
    {
        fclose(file);
    }

    virtual std::streampos Seek(std::streamoff off, std::ios::seekdir dir = std::ios::beg) override
    {
        if (dir == std::ios::cur)
        {
            fseek(file, off, SEEK_CUR);
        }
        else if (dir == std::ios::end)
        {
            fseek(file, off, SEEK_END);
        }
        else
        {
            fseek(file, off, SEEK_SET);
        }
        return ftell(file);
    }

    virtual std::streampos Tell() override
    {
        return std::streampos(ftell(file));
    }

    virtual bool Read(void* dest, std::size_t len) override
    {
        if (mode == BStreamMode::Write) return false;
        return fread(dest, 1, len, file) == len;
    }

    virtual bool Write(const void* src, std::size_t len) override
    {
        if (mode == BStreamMode::Read) return false;
        return fwrite(src, 1, len, file) == len;
    }

};