#include "stringHash.hpp"

#include "stringTable.hpp"

void BYMLNodeDataStringHash::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataStringHash::Read");
    u24 size = *src.ReadU24();
    for (u32 i = 0; i < size.Get(); i++)
    {
        std::string key = ctx.hashTable->strs[src.ReadU24()->Get()];
        BYMLType type((BYMLType)*src.Read<u8>());
        std::streampos bak = src.Tell();
        auto& node = map.emplace(key, BYMLNode()).first->second;
        if (BYMLNode::IsValue(type))
        {
            node.ReadData(src, type, BYMLNodeReadCtx(bak, ctx.filePos, ctx.hashTable, ctx.stringTable));
        }
        else
        {
            std::streampos absOff = ctx.filePos + std::streamoff(*src.Read<u32>());
            src.Seek(absOff);
            node.type = (BYMLType)*src.Read<u8>(); // Idk.
            node.ReadData(src, type, BYMLNodeReadCtx(src.Tell(), ctx.filePos, ctx.hashTable, ctx.stringTable));
        }
        src.Seek(bak + std::streamoff(4));
    }
}

void BYMLNodeDataStringHash::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataStringHash::Write");
    // TODO!!!
    abort();
}

void BYMLNodeDataStringHash::EmitYAML(YAML::Emitter& node)
{
    ZoneScopedN("BYMLNodeDataStringHash::EmitYAML");
    node << YAML::BeginMap;
    for (auto& item : map)
    {
        node << YAML::Key << item.first << YAML::Value;
        item.second.data->EmitYAML(node);
    }
    node << YAML::EndMap;
}