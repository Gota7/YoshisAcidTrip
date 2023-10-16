#include "array.hpp"

void BYMLNodeDataArray::Read(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNodeDataArray::Read");
    u24 size = *src.ReadU24();
    nodes.reserve(size.Get());
    std::streampos begPos = src.Tell();
    std::streampos typeEndPos = begPos + std::streamoff(size.Get());
    while ((typeEndPos - begPos) % 4 != 0)
    {
        typeEndPos = typeEndPos + std::streamoff(1);
    } // Pad.
    for (u32 i = 0; i < size.Get(); i++)
    {
        BYMLType type((BYMLType)*src.Read<u8>());
        std::streampos bak = src.Tell();
        src.Seek(typeEndPos + std::streamoff(4 * i));

        // Read type.
        if (BYMLNode::IsValue(type))
        {
            nodes.emplace_back();
            nodes.back()->ReadData(src, type, BYMLNodeReadCtx(src.Tell(), ctx.filePos, ctx.hashTable, ctx.stringTable));
        }
        else
        {
            std::streampos absOff = ctx.filePos + std::streamoff(*src.Read<u32>());
            src.Seek(absOff);
            nodes.emplace_back(JPtrMake(BYMLNode, src, BYMLNodeReadCtx(absOff, ctx.filePos, ctx.hashTable, ctx.stringTable)));
        }
        src.Seek(bak);
    }
}

void BYMLNodeDataArray::Write(BStream& dst)
{
    ZoneScopedN("BYMLNodeDataArray::Write");
    // TODO!!!
    abort();
}

void BYMLNodeDataArray::EmitYAML(YAML::Emitter& node)
{
    ZoneScopedN("BYMLNodeDataArray::EmitYAML");
    node << YAML::BeginSeq;
    for (auto& item : nodes)
    {
        item->data->EmitYAML(node);
    }
    node << YAML::EndSeq;
}