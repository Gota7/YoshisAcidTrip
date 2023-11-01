#include "uint64.hpp"

void BYMLNodeDataUint64::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataUint64::Read");
    val = *src.Read<uint64_t>();
}

void BYMLNodeDataUint64::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataUint64::Write");
    dst.Write(val);
}

void BYMLNodeDataUint64::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataUint64::EmitYAML");
    node << YAML::LocalTag("ul") << val;
}