#pragma once

#include "../node.hpp"

// Uint64.
struct BYMLNodeDataUint64 : BYMLNodeData
{
    uint64_t val;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_UINT64; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;
    virtual ~BYMLNodeDataUint64() override {}

};