#include "int.hpp"

void BYMLNodeDataInt::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataInt::Read");
    val = *src.Read<int32_t>();
}

void BYMLNodeDataInt::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataInt::Write");
    dst.Write(val);
}

void BYMLNodeDataInt::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataInt::EmitYAML");
    node << YAML::LocalTag("l") << val;
}