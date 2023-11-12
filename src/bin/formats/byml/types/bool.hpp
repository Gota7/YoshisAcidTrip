#pragma once

#include "../node.hpp"

// Bool.
struct BYMLNodeDataBool : BYMLNodeData
{
    bool val;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_BOOL; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;
    virtual ~BYMLNodeDataBool() override {}

};