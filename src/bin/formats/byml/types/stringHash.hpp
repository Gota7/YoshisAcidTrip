#pragma once

#include "../node.hpp"

// String hash.
struct BYMLNodeDataStringHash : BYMLNodeData
{
    std::map<std::string, BYMLNode> map;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_STRING_HASH; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;

};