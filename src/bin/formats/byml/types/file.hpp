#pragma once

#include "../../../../jsystem/fs.hpp"
#include "../node.hpp"

// File.
struct BYMLNodeDataFile : BYMLNodeData
{
    std::vector<u8> data;
    u32 unk = 0x1000;

    // V-functions.
    virtual BYMLType Type() override { return BYML_TYPE_FILE; };
    virtual void Read(BStream& src, const BYMLNodeReadCtx& ctx) override;
    virtual void Write(BStream& dst) override;
    virtual void EmitYAML(YAML::Emitter& node, const JResPath& basePath, int& currFileInd) override;

};