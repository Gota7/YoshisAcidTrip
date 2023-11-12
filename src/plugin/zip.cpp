#include "zip.hpp"

#include "../bin/streams/file.hpp"
#include "../common.hpp"
#include <filesystem>

PZIP::PZIP(const std::string& path)
{
    ZoneScopedN("PZIP::PZIP");
    folderPath = path.substr(0, path.size() - 4);
    std::filesystem::path folder(folderPath); // Without extension.
    if (std::filesystem::exists(folder) && std::filesystem::is_directory(folder))
    {
        folderMode = true;
    }
    else
    {
        zip = zip_open(path.c_str(), 7, 'r');
        folderMode = false;
    }
}

JPtr<BStream> PZIP::Open(const std::string& path)
{
    ZoneScopedN("PZIP::Open");
    if (folderMode)
    {
        return JPtrMake(BStreamFile, JResPath(folderPath + "/" + path, true), BStreamMode::Read);
    }
    else
    {
        if (zip_entry_open(zip, path.c_str())) return nullptr;
        std::vector<char> data;
        data.resize(zip_entry_noallocread(zip, data.data(), data.size()));
        zip_entry_close(zip);
        return JPtrMake(BStreamMemory, BStreamMode::Read, std::move(data));
    }
}

std::optional<std::string> PZIP::String(const std::string& path)
{
    ZoneScopedN("PZIP::String");
    if (folderMode)
    {
        auto file = Open(path);
        if (!file) return std::nullopt;
        else return file->ReadFixedLenStr(file->Size());
    }
    else
    {
        if (zip_entry_open(zip, path.c_str())) return std::nullopt;
        std::vector<char> data;
        data.resize(zip_entry_size(zip));
        zip_entry_noallocread(zip, data.data(), data.size());
        zip_entry_close(zip);
        data.push_back(0);
        return std::string(data.data());
    }
}

// ssize_t PZIP::FileSize(const std::string& path)
// {
//     ZoneScopedN("PZIP::FileSize");
//     if (zip_entry_open(zip, path.c_str())) return -1;
//     ssize_t ret = (ssize_t)zip_entry_size(zip);
//     zip_entry_close(zip);
//     return ret;
// }

// ssize_t PZIP::Read(const std::string& path, void* buf)
// {
//     ZoneScopedN("PZIP::Read");
//     if (zip_entry_open(zip, path.c_str())) return -1;
//     ssize_t ret = zip_entry_noallocread(zip, buf, zip_entry_size(zip));
//     zip_entry_close(zip);
//     return ret;
// }

std::vector<std::string> PZIP::FileList()
{
    ZoneScopedN("PZIP::FileList");
    std::vector<std::string> ret;
    if (folderMode)
    {
        std::filesystem::recursive_directory_iterator iter(folderPath);
        for (auto file : iter)
        {
            std::string path = file.path().string();
            ret.push_back(path.substr(folderPath.size() + 1, path.size()));
        }
    }
    else
    {
        int cnt = zip_entries_total(zip);
        ret.reserve(cnt);
        for (int i = 0; i < cnt; i++)
        {
            zip_entry_openbyindex(zip, i);
            ret.push_back(zip_entry_name(zip));
            zip_entry_close(zip);
        }
    }
    return ret;
}

PZIP::~PZIP()
{
    ZoneScopedN("PZIP::~PZIP");
    if (!folderMode) zip_close(zip);
}