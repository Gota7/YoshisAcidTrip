#pragma once

#include <common.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

// Prevent bad habit of mixing up full-paths and relative paths.
struct JResPath
{
    std::string fullPath;

    // Make a path to a resource from a relative path (converts "tes/test.txt" to "/home/gota/Documents/C++/Juno/res/tes/test.txt" for example). Force means to not convert full path, do NOT do unless you know what you are doing!
    JResPath(const std::string& relPath, bool force = false);

};

// Static class for loading files.
namespace JFileSystem
{

    // Convert a path to use proper path seperators.
    std::string PathSeparators(const std::string& path);

    // Get a proper absolute file path.
    std::string AbsPath(const std::string& path);

    // If a file exists.
    inline bool FileExists(const JResPath& path)
    {
        return std::filesystem::exists(path.fullPath);
    }

    // Delete a file.
    inline bool Delete(const JResPath& path, bool recursive = false)
    {
        if (recursive) return std::filesystem::remove_all(path.fullPath) > 0;
        else return std::filesystem::remove(path.fullPath);
    }

    // Move/rename a file.
    inline void Rename(const JResPath& path, const JResPath& newPath)
    {
        std::filesystem::rename(path.fullPath, newPath.fullPath);
    }

    // Get the parent directory of a file.
    inline std::string ParentDir(const std::string& path)
    {
        std::string relativeDirectory = path.substr(0, path.find_last_of('/'));
        if (relativeDirectory != "") relativeDirectory += "/";
        return relativeDirectory;
    }

    // Load an in file stream. Remember to close it.
    inline std::ifstream OpenFile(const JResPath& path)
    {
        return std::ifstream(path.fullPath);
    }

    // Load an out file stream. Remember to close it.
    inline std::ofstream SaveFile(const JResPath& path)
    {
        return std::ofstream(path.fullPath);
    }

    // Load all text from a file.
    inline std::string ReadAllLines(const JResPath& path)
    {
        auto ifs = OpenFile(path);
        std::string ret((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        ifs.close();
        return ret;
    }

    // Read a file.
    inline std::vector<char> ReadAllBytes(const JResPath& path)
    {
        auto ifs = std::ifstream(path.fullPath, std::ios::binary);
        std::vector<char> ret((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        ifs.close();
        return ret;
    }

    // Read a null terminated string.
    inline std::string ReadNullTerminated(std::ifstream& file)
    {
        char curr;
        std::string ret = "";
        do
        {
            file.read(&curr, 1);
            if (curr != 0) ret += curr;
        } while (curr != 0);
        return ret;
    }

    // Write a null terminated string.
    inline void WriteNullTerminated(std::ofstream& file, const std::string& str)
    {
        file.write(str.data(), str.size() + 1);
    }

    // Read from a file.
    template <typename T>
    inline T Read(std::ifstream& file)
    {
        T ret;
        file.read((char*)&ret, sizeof(T));
        return ret;
    }

    // Read from a file to something.
    template <typename T>
    inline void ReadTo(std::ifstream& file, T* to, size_t cnt = 1)
    {
        file.read((char*)to, sizeof(T) * cnt);
    }

    // Write to a file.
    template <typename T>
    inline void Write(std::ofstream& file, const T& val)
    {
        file.write((char*)&val, sizeof(T));
    }

};