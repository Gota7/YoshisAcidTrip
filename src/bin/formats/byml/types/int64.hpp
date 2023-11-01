#pragma once

#include "../node.hpp"

// Int64.
struct BYMLNodeDataInt64 : BYMLNodeData
{
    int64_t val;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_INT64; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;

};