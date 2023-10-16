#pragma once

#include "../node.hpp"

// Int.
struct BYMLNodeDataInt : BYMLNodeData
{
    int val;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_INT; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node) override;

};