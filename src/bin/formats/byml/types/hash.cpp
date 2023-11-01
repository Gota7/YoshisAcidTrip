#include "hash.hpp"

void BYMLNodeDataHash::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataHash::Read");
    u24 size = *src.ReadU24();
    std::streampos typesPos = src.Tell() + std::streamoff(8 * size.Get());
    for (u32 i = 0; i < size.Get(); i++) // TODO!!!
    {
        uint32_t key = *src.Read<u32>();
        std::streampos bak = src.Tell();
        src.Seek(typesPos + std::streamoff(i));
        BYMLType type((BYMLType)*src.Read<u8>());
        src.Seek(bak);
        auto& node = map.emplace(key, BYMLNode()).first->second;
        if (BYMLNode::IsValue(type))
        {
            node.ReadData(src, type, BYMLNodeReadCtx(bak, ctx.filePos, ctx.hashTable, ctx.stringTable));
        }
        else
        {
            std::streampos absOff = ctx.filePos + std::streamoff(*src.Read<u32>());
            src.Seek(absOff);
            node.type = BYMLNode::IsSpecial(type) ? type : ((BYMLType)*src.Read<u8>());
            node.ReadData(src, type, BYMLNodeReadCtx(src.Tell(), ctx.filePos, ctx.hashTable, ctx.stringTable));
        }
        src.Seek(bak + std::streamoff(4));
    }
}

void BYMLNodeDataHash::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataHash::Write");
    // TODO!!!
    abort();
}

void BYMLNodeDataHash::EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd)
{
    ZoneScopedN("BYMLNodeDataHash::EmitYAML");
    node << YAML::BeginMap;
    for (auto& item : map)
    {
        node << YAML::Key << item.first << YAML::Value;
        item.second.data->EmitYAML(node, basePath, currFileInd);
    }
    node << YAML::EndMap;
}