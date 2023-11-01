#include "file.hpp"

#include "../../../streams/file.hpp"

void BYMLNodeDataFile::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataFile::Read");
    u32 size = *src.Read<u32>();
    unk = *src.Read<u32>();
    data.resize(size);
    src.Read(data.data(), size);
}

void BYMLNodeDataFile::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataFile::Write");
    dst.Write((u32)data.size());
    dst.Write(data.data(), data.size());
}

void BYMLNodeDataFile::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataFile::EmitYAML");
    node << YAML::LocalTag("file") << YAML::Flow << YAML::BeginMap;
    std::string filePath = basePath.fullPath + "." + std::to_string(currFileInd);
    BStreamFile file(JResPath(filePath, true), BStreamMode::Write);
    file.Write(data.data(), data.size());
    node << YAML::Key << "FileInd" << YAML::Value << currFileInd++;
    node << YAML::Key << "Unk" << YAML::Hex << YAML::Value << unk;
    node << YAML::EndMap;
}