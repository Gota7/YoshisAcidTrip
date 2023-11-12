#pragma once

#include "../node.hpp"

// String.
struct BYMLNodeDataString : BYMLNodeData
{
    std::string val;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_STRING; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;
    virtual ~BYMLNodeDataString() override {}

};