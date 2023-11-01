#pragma once

#include "../node.hpp"

// Array.
struct BYMLNodeDataArray : BYMLNodeData
{
    std::vector<JPtr<BYMLNode>> nodes;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_ARRAY; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;

};