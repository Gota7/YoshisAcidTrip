#pragma once

#include "../node.hpp"

// Hash.
struct BYMLNodeDataHash : BYMLNodeData
{
    std::map<uint32_t, BYMLNode> map;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_HASH; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;

};