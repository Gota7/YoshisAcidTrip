#pragma once

#include "../node.hpp"

// String table.
struct BYMLNodeDataStringTable : BYMLNodeData
{
    std::vector<std::string> strs;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_STRING_TABLE; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;
    virtual ~BYMLNodeDataStringTable() override {}

};