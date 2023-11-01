#include "bool.hpp"

void BYMLNodeDataBool::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataBool::Read");
    val = *src.Read<u32>() > 0;
}

void BYMLNodeDataBool::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataBool::Write");
    dst.Write((u32)val);
}

void BYMLNodeDataBool::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataBool::EmitYAML");
    node << val;
}