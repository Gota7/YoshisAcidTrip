#pragma once

#include "../node.hpp"

// Uint.
struct BYMLNodeDataUint : BYMLNodeData
{
    uint32_t val;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_UINT; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;

};