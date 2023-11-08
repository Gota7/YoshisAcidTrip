#include "bin.hpp"

void BYMLNodeDataBin::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataBin::Read");
    u32 size = *src.Read<u32>();
    data.resize(size);
    src.Read(data.data(), size);
}

void BYMLNodeDataBin::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataBin::Write");
    dst.Write((u32)data.size());
    dst.Write(data.data(), data.size());
}

void BYMLNodeDataBin::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataBin::EmitYAML");
    node << YAML::LocalTag("bin") << YAML::Flow << YAML::BeginSeq;
    for (auto& byte : data)
    {
        node << YAML::Hex << ((unsigned int)byte);
    }
    node << YAML::EndSeq;
}