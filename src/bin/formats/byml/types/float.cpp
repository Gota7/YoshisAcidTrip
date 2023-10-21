#include "float.hpp"

void BYMLNodeDataFloat::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataFloat::Read");
    val = *src.Read<float>();
}

void BYMLNodeDataFloat::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataFloat::Write");
    dst.Write(val);
}

void BYMLNodeDataFloat::EmitYAML(YAML::Emitter& node)
{
    ZoneScopedN("BYMLNodeDataFloat::EmitYAML");
    node << YAML::LocalTag("f32") << val;
}