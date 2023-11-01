#include "string.hpp"

#include "stringTable.hpp"

void BYMLNodeDataString::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataString::Read");
    val = ctx.stringTable->strs[*src.Read<u32>()];
}

void BYMLNodeDataString::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataString::Write");
    // TODO!!!
    abort();
}

void BYMLNodeDataString::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataString::EmitYAML");
    node << YAML::LocalTag("str") << val;
}