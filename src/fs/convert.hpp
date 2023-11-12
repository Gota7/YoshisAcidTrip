#pragma once

#include "conversionFile.hpp"
#include "romfs.hpp"

// Conversion progress context.
struct FConvertProgressCtx
{
    JPtr<FConversionFile> file;
    std::string convStepDesc = "Starting conversion...";
    std::size_t currStep = 0;
    std::size_t numSteps = 1;
    unsigned int version = 0;
    bool init = true;
    bool stepCompleted = true;
    bool full = false;
};

// Convert a ROMFS.
namespace FConvert
{

    // Make an editor-compatible ROMFS. Note that this a cooperative function so you need to run it multiple times until it is done!
    std::optional<FRomfs> MakeEditorRomfs(const JResPath& srcDir, const JResPath& baseDir, const JResPath& patchDir, FConvertProgressCtx& ctx);

    // Update ROMFS to new version. Note that this a cooperative function so you need to run it multiple times until it is done! Returns true if no error.
    bool UpdateRomfs(FRomfs& romfs, FConvertProgressCtx& ctx, bool forceReFileChecks = false);

    // Compile ROMFS.
    void CompileRomfs(const FRomfs& romfs, const JResPath& dstDir);

};