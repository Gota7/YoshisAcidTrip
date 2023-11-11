#include "zip.hpp"

#include "../common.hpp"

PZIP::PZIP(const std::string& path)
{
    ZoneScopedN("PZIP::PZIP");
    zip = zip_open(path.c_str(), 7, 'r');
}

std::optional<BStreamMemory> PZIP::Open(const std::string& path)
{
    ZoneScopedN("PZIP::Open");
    if (zip_entry_open(zip, path.c_str())) return std::nullopt;
    std::vector<char> data;
    data.resize(zip_entry_noallocread(zip, data.data(), data.size()));
    zip_entry_close(zip);
    return std::optional<BStreamMemory>(BStreamMemory(BStreamMode::Read, std::move(data)));
}

std::optional<std::string> PZIP::String(const std::string& path)
{
    ZoneScopedN("PZIP::String");
    if (zip_entry_open(zip, path.c_str())) return std::nullopt;
    std::vector<char> data;
    data.resize(zip_entry_size(zip));
    zip_entry_noallocread(zip, data.data(), data.size());
    zip_entry_close(zip);
    data.push_back(0);
    return std::string(data.data());
}

ssize_t PZIP::FileSize(const std::string& path)
{
    ZoneScopedN("PZIP::FileSize");
    if (zip_entry_open(zip, path.c_str())) return -1;
    ssize_t ret = (ssize_t)zip_entry_size(zip);
    zip_entry_close(zip);
    return ret;
}

ssize_t PZIP::Read(const std::string& path, void* buf)
{
    ZoneScopedN("PZIP::Read");
    if (zip_entry_open(zip, path.c_str())) return -1;
    ssize_t ret = zip_entry_noallocread(zip, buf, zip_entry_size(zip));
    zip_entry_close(zip);
    return ret;
}

std::vector<std::string> PZIP::FileList()
{
    ZoneScopedN("PZIP::FileList");
    int cnt = zip_entries_total(zip);
    std::vector<std::string> ret;
    ret.reserve(cnt);
    for (int i = 0; i < cnt; i++)
    {
        zip_entry_openbyindex(zip, i);
        ret.push_back(zip_entry_name(zip));
        zip_entry_close(zip);
    }
    return ret;
}

PZIP::~PZIP()
{
    ZoneScopedN("PZIP::~PZIP");
    zip_close(zip);
}