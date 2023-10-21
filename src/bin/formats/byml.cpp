#include "byml.hpp"

BYMLNode::BYMLNode(BStream& src, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNode::BYMLNode");
    std::streampos basePos = src.Tell();
    u8 type = *src.Read<u8>();
    ReadData(src, (BYMLType)type, BYMLNodeReadCtx(basePos, ctx.filePos, ctx.hashTable, ctx.stringTable));
}

void BYMLNode::ReadData(BStream& src, BYMLType type, const BYMLNodeReadCtx& ctx)
{
    ZoneScopedN("BYMLNode::ReadData");
    switch (type)
    {
        case BYML_TYPE_STRING:
        {
            data = JPtrMake(BYMLNodeDataString);
            break;
        }
        case BYML_TYPE_ARRAY:
        {
            data = JPtrMake(BYMLNodeDataArray);
            break;
        }
        case BYML_TYPE_STRING_HASH:
        {
            data = JPtrMake(BYMLNodeDataStringHash);
            break;
        }
        case BYML_TYPE_STRING_TABLE:
        {
            data = JPtrMake(BYMLNodeDataStringTable);
            break;
        }
        case BYML_TYPE_BOOL:
        {
            data = JPtrMake(BYMLNodeDataBool);
            break;
        }
        case BYML_TYPE_INT:
        {
            data = JPtrMake(BYMLNodeDataInt);
            break;
        }
        case BYML_TYPE_FLOAT:
        {
            data = JPtrMake(BYMLNodeDataFloat);
            break;
        }
        default:
        {
            DBG_PRINT("BYML@LSD: Unhandled node type 0x" << std::hex << (int)type << " found at 0x" << std::hex << src.Tell() << ".");
            abort();
        }
    }
    data->Read(src, ctx);
}

BYML::BYML(BStream& src)
{
    ZoneScopedN("BYML::BYML");
    std::streampos baseOff = src.Tell();

    // Determine endian.
    src.endian = BStreamEndian::Big;
    auto maybeMagic = src.ReadFixedLenStr(2);
    if (!maybeMagic)
    {
        DBG_PRINT("BYML@LSD: Unreadable stream.");
        return;
    }
    if (*maybeMagic == std::string("BY")) {}
    else if (*maybeMagic == std::string("YB"))
    {
        src.endian = BStreamEndian::Little;
    }
    else
    {
        DBG_PRINT("BYML@LSD: Invalid BYML or unreadable stream! Recieved magic \"" << *maybeMagic << "\".");
        return;
    }

    // Header stuff.
    u16 version = *src.Read<u16>();
    if (version > 7)
    {
        DBG_PRINT("BYML@LSD: BYML version \"" << version << "\" is not supported.");
        return;
    }
    std::streampos hashKeyTableOff = baseOff + std::streamoff(*src.Read<u32>());
    std::streampos stringTableOff = baseOff + std::streamoff(*src.Read<u32>());
    std::streampos rootNodeOff = baseOff + std::streamoff(*src.Read<u32>());
    JPtr<BYMLNode> hashKeyTable = nullptr;
    JPtr<BYMLNode> stringTable = nullptr;
    rootNode = nullptr;
    if (hashKeyTableOff != baseOff)
    {
        src.Seek(hashKeyTableOff);
        hashKeyTable = JPtrMake(BYMLNode, src, BYMLNodeReadCtx(baseOff, baseOff, nullptr, nullptr));
    }
    if (stringTableOff != baseOff)
    {
        src.Seek(stringTableOff);
        stringTable = JPtrMake(BYMLNode, src, BYMLNodeReadCtx(baseOff, baseOff, (BYMLNodeDataStringTable*)hashKeyTable->data.get(), nullptr));
    }
    if (rootNodeOff != baseOff)
    {
        src.Seek(rootNodeOff);
        rootNode = JPtrMake(BYMLNode, src, BYMLNodeReadCtx(baseOff, baseOff, (BYMLNodeDataStringTable*)hashKeyTable->data.get(), (BYMLNodeDataStringTable*)stringTable->data.get()));
    }

}

void BYML::WriteYAML(const JResPath& path)
{
    ZoneScopedN("BYML::WriteYAML");
    std::ofstream file = std::ofstream(path.fullPath);
    file.clear();
    YAML::Emitter root(file);
    if (rootNode) rootNode->data->EmitYAML(root);
    file.close();
}