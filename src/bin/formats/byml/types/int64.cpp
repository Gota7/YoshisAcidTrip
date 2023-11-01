#include "int64.hpp"

void BYMLNodeDataInt64::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataInt64::Read");
    val = *src.Read<int64_t>();
}

void BYMLNodeDataInt64::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataInt64::Write");
    dst.Write(val);
}

void BYMLNodeDataInt64::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataInt64::EmitYAML");
    node << YAML::LocalTag("ll") << val;
}