#include "int.hpp"

void BYMLNodeDataInt::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataInt::Read");
    val = *src.Read<int>();
}

void BYMLNodeDataInt::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataInt::Write");
    dst.Write(val);
}

void BYMLNodeDataInt::EmitYAML(YAML::Emitter& node)
{
    ZoneScopedN("BYMLNodeDataInt::EmitYAML");
    node << YAML::LocalTag("i32") << val;
}