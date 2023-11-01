#pragma once

#include "../../../jsystem/fs.hpp"
#include "../../../jsystem/mem.hpp"
#include "../../stream.hpp"
#include <map>
#include <yaml-cpp/yaml.h>

// Node types.
enum BYMLType : u8
{
    BYML_TYPE_INVALID = 0x00,
    BYML_TYPE_HASH = 0x20,
    BYML_TYPE_STRING = 0xA0,
    BYML_TYPE_BIN = 0xA1,
    BYML_TYPE_FILE = 0xA2,
    BYML_TYPE_ARRAY = 0xC0,
    BYML_TYPE_STRING_HASH = 0xC1,
    BYML_TYPE_STRING_TABLE = 0xC2,
    BYML_TYPE_BOOL = 0xD0,
    BYML_TYPE_INT = 0xD1,
    BYML_TYPE_FLOAT = 0xD2,
    BYML_TYPE_UINT = 0xD3,
    BYML_TYPE_INT64 = 0xD4,
    BYML_TYPE_UINT64 = 0xD5,
    BYML_TYPE_DOUBLE = 0xD6
};

// Forward declare.
struct BYMLNodeDataStringTable;

// Reading context.
struct BYMLNodeReadCtx
{
    std::streampos basePos;
    std::streampos filePos;
    BYMLNodeDataStringTable* hashTable;
    BYMLNodeDataStringTable* stringTable;

    // Create new context.
    BYMLNodeReadCtx(std::streampos basePos, std::streampos filePos, BYMLNodeDataStringTable* hashTable, BYMLNodeDataStringTable* stringTable) : basePos(basePos), filePos(filePos), hashTable(hashTable), stringTable(stringTable) {}

};

// Generic node data functions.
struct BYMLNodeData
{

    // Get node type.
    virtual BYMLType Type() = 0;

    // Read from a stream.
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) = 0;

    // Write to a stream.
    virtual void Write(BStream& dst) = 0;

    // Emit YAML. Note that the base path is the base file name of the files to save.
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) = 0;

};

// BYML Node.
struct BYMLNode
{
    BYMLType type;
    JPtr<BYMLNodeData> data = nullptr;

    // Default.
    BYMLNode() {}

    // Read a node.
    BYMLNode(BStream& src, const BYMLNodeReadCtx& ctx, BYMLType specialTypeDontRead = BYML_TYPE_INVALID);

    // Read node data.
    void ReadData(BStream& src, BYMLType type, const BYMLNodeReadCtx& ctx);

    // If a type is a value that fits in 4 bytes.
    static bool IsValue(BYMLType type)
    {
        switch (type)
        {
            case BYML_TYPE_STRING:
            case BYML_TYPE_BOOL:
            case BYML_TYPE_INT:
            case BYML_TYPE_FLOAT:
            case BYML_TYPE_UINT:
                return true;
            default:
                return false;
        }
    }

    // If a type is a special value type (does not have type header).
    static bool IsSpecial(BYMLType type)
    {
        switch (type)
        {
            case BYML_TYPE_BIN:
            case BYML_TYPE_FILE:
            case BYML_TYPE_INT64:
            case BYML_TYPE_UINT64:
            case BYML_TYPE_DOUBLE:
                return true;
            default:
                return false;
        }
    }

};