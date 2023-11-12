#include <bin/formats/sarc.hpp>

// Nodes for an item.
struct SFATNode
{
    u32 nameHash;
    u32 fileAttributes;
    u32 dataStartOff;
    u32 dataEndOff;
    DEFINE_FIELDS(SFATNode, nameHash, fileAttributes, dataStartOff, dataEndOff);
};

SARC::SARC(BStream& src)
{
    ZoneScopedN("SARC::SARC");

    // Header info.
    std::streampos baseOff = src.Tell();
    auto maybeMagic = src.ReadFixedLenStr(4);
    if (!maybeMagic)
    {
        DBG_PRINT("SARC@LSD: Unreadable stream.");
        return;
    }
    if (*maybeMagic != std::string("SARC"))
    {
        DBG_PRINT("SARC@LSD: Invalid SARC or unreadable stream! Recieved magic \"" << *maybeMagic << "\".");
        return;
    }
    src.Read<u16>(); // Header length.
    BStreamEndian bak = src.endian;
    src.endian = BStreamEndian::Big;
    if (*src.Read<u16>() == 0xFFFE) src.endian = BStreamEndian::Little;
    src.Read<u32>(); // File size.
    std::streampos dataOff = baseOff + std::streampos(*src.Read<s32>());
    src.Read<u32>();

    // SFAT section.
    src.Read<u32>(); // Header.
    src.Read<u16>(); // Header length.
    u16 numNodes = *src.Read<u16>();
    src.Read<u32>(); // Hash key.
    std::vector<SFATNode> nodes;
    nodes.resize(numNodes);
    for (u16 i = 0; i < numNodes; i++)
    {
        nodes[i] = *src.Read<SFATNode>();
    }

    // Read all files.
    src.Read<u64>(); // Header.
    std::streampos sfntOff = src.Tell();
    int cnt = 0;
    for (auto& node : nodes)
    {
        std::string fileName = "#" + std::to_string(cnt);
        if (node.fileAttributes & 0x01000000)
        {
            src.Seek(sfntOff + std::streampos((node.fileAttributes & 0xFFFF) * 4));
            fileName = *src.ReadCStr();
        }
        std::size_t len = node.dataEndOff - node.dataStartOff;
        src.Seek(dataOff + std::streamoff(node.dataStartOff));
        auto added = files.emplace(fileName, BStreamMode::ReadWrite);
        added.first->second.Copy(src, len);
        cnt++;
    }

    // Backup endian.
    src.endian = bak;

}

void SARC::Write(BStream& dst)
{
    ZoneScopedN("SARC::Write");
    assert(false); // TODO!!!
}

BStream& SARC::Open(const std::string& path, BStreamMode mode)
{
    ZoneScopedN("SARC::Open");
    auto file = files.find(path);
    if (file == files.end())
    {
        file = files.emplace(path, BStreamMode::ReadWrite).first;
        assert(false);
    }
    file->second.Seek(0);
    return file->second;
}

bool SARC::Rename(const std::string& oldPath, const std::string& newPath)
{
    ZoneScopedN("SARC::Rename");
    auto file = files.find(oldPath);
    if (file == files.end()) return false;
    files.emplace(newPath, std::move(file->second));
    files.erase(file);
    return true;
}

bool SARC::Delete(const std::string& path)
{
    ZoneScopedN("SARC::Delete");
    auto file = files.find(path);
    if (file != files.end())
    {
        files.erase(file);
        return true;
    }
    else return false;
}

bool SARC::FileExists(const std::string& path)
{
    ZoneScopedN("SARC::FileExists");
    return files.find(path) != files.end();
}

std::size_t SARC::NumFiles()
{
    ZoneScopedN("SARC::NumFiles");
    return files.size();
}

std::vector<std::string> SARC::GetFilenames()
{
    ZoneScopedN("SARC::GetFilenames");
    std::vector<std::string> ret;
    for (auto& file : files)
    {
        ret.push_back(file.first);
    }
    return ret;
}
