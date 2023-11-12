#pragma once

#include "../node.hpp"

// Float.
struct BYMLNodeDataFloat : BYMLNodeData
{
    float val;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_FLOAT; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;
    virtual ~BYMLNodeDataFloat() override {}

};