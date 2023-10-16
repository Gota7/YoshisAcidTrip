#include "stringTable.hpp"

void BYMLNodeDataStringTable::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataStringTable::Read");
    u24 size = *src.ReadU24();
    strs.reserve(size.Get());
    for (u32 i = 0; i < size.Get(); i++)
    {
        std::streamoff off(*src.Read<u32>());
        std::streampos bak = src.Tell();
        src.Seek(ctx.basePos + off);
        strs.push_back(*src.ReadCStr());
        src.Seek(bak);
    }
}

void BYMLNodeDataStringTable::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataStringTable::Write");
    // TODO!!!
    abort();
}

void BYMLNodeDataStringTable::EmitYAML(YAML::Emitter& node)
{
    ZoneScopedN("BYMLNodeDataStringTable::EmitYAML");
    // TODO!!!
    abort();
}