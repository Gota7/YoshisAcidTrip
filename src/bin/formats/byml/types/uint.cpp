#include "uint.hpp"

void BYMLNodeDataUint::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataUint::Read");
    val = *src.Read<uint32_t>();
}

void BYMLNodeDataUint::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataUint::Write");
    dst.Write(val);
}

void BYMLNodeDataUint::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataUint::EmitYAML");
    node << YAML::LocalTag("u") << val;
}