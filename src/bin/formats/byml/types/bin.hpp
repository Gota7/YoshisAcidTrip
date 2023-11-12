#pragma once

#include "../node.hpp"

// Bin.
struct BYMLNodeDataBin : BYMLNodeData
{
    std::vector<u8> data;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_BIN; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;
    virtual ~BYMLNodeDataBin() override {}

};