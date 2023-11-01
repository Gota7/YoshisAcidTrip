#include "double.hpp"

void BYMLNodeDataDouble::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataDouble::Read");
    val = *src.Read<double>();
}

void BYMLNodeDataDouble::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataDouble::Write");
    dst.Write(val);
}

void BYMLNodeDataDouble::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataDouble::EmitYAML");
    node << YAML::LocalTag("d") << val;
}