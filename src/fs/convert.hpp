#pragma once

#include "romfs.hpp"

// Convert a ROMFS.
namespace FConvert
{

    // Make an editor-compatible ROMFS.
    std::optional<FRomfs> MakeEditorRomfs(const JResPath& srcDir, const JResPath& baseDir, const JResPath& patchDir);

    // Update ROMFS to new version.
    bool UpdateRomfs(FRomfs& romfs, bool forceReFileChecks = false);

    // Compile ROMFS.
    void CompileRomfs(const FRomfs& romfs, const JResPath& dstDir);

};