#include "convert.hpp"

#include "../bin/formats/byml.hpp"
#include "../bin/formats/sarc.hpp"
#include "../bin/formats/zstd.hpp"
#include <functional>

#define MAX_NUM_STEPS 10

// Using an output file and one with extension removed, debinarize into input files.
using FConvertItemFunc = std::function<std::pair<std::vector<FConversionFileInputMode>, std::vector<FConversionFileInput>>(FRomfs&, const std::string&, const std::string&)>;

inline bool FConvertEndsWith(std::string const& value, std::string const& ending)
{
    ZoneScopedN("FConvertEndsWith");
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::optional<FRomfs> FConvert::MakeEditorRomfs(const JResPath& srcDir, const JResPath& baseDir, const JResPath& patchDir, FConvertProgressCtx& ctx)
{
    ZoneScopedN("FConvert::MakeEditorRomfs");

    // Init.
    std::optional<FRomfs> ret;
    if (ctx.init)
    {

        // Make sure source exists.
        if (!std::filesystem::exists(srcDir.fullPath)) return std::nullopt;

        // Make directories.
        std::filesystem::create_directories(baseDir.fullPath);
        std::filesystem::create_directories(patchDir.fullPath);
        std::filesystem::copy(srcDir.fullPath, baseDir.fullPath, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);

        // Add initial version.
        ret = FRomfs(baseDir, patchDir);
        if (!ctx.file) ctx.file = JPtrMake(FConversionFile, *ret, true);
        ctx.file->Save(*ret);
        ctx.init = false;

    }
    else ret = FRomfs(baseDir, patchDir);
    UpdateRomfs(*ret, ctx);
    return ret;

}

// Convert an item.
void FConvertItem(FRomfs& romfs, FConversionFileStep& step, const std::vector<std::string>& exts, const std::string& command, const FConvertItemFunc convFunc)
{
    ZoneScopedN("FConvertItem");

    // Replace copy. Copy is only allowed to work on single files, just need to make sure it isn't from a step. Would be inefficient from a step but idk maybe it happens.
    if (step.command == "copy" && step.inputSteps.size() > 0 && step.inputSteps[0] == FConversionFileInputMode::String)
    {
        std::string filePath = std::get<std::string>(step.inputFiles[0]);
        std::string foundExt = "";
        for (auto& ext : exts)
        {
            if (FConvertEndsWith(filePath, ext))
            {
                foundExt = ext;
                break;
            }
        }
        if (foundExt == "") return; // No extension found.
        step.command = command;
        auto items = convFunc(romfs, filePath, filePath.substr(0, filePath.size() - foundExt.size()));
        step.inputSteps = items.first;
        step.inputFiles = items.second;
        return;
    }

    // Standard input files.
    for (std::size_t i = 0; i < step.inputFiles.size(); i++)
    {

        // The input is a dead end and can be converted.
        if (step.inputSteps[i] == FConversionFileInputMode::String)
        {
            std::string filePath = std::get<std::string>(step.inputFiles[i]);
            std::string foundExt = "";
            for (auto& ext : exts)
            {
                if (FConvertEndsWith(filePath, ext))
                {
                    foundExt = ext;
                    break;
                }
            }
            if (foundExt == "") continue; // No extension found, we can not convert this.

            // Convert to step.
            step.inputSteps[i] = FConversionFileInputMode::Step;
            auto items = convFunc(romfs, filePath, filePath.substr(0, filePath.size() - foundExt.size()));
            step.inputFiles[i] = FConversionFileStep(items.second, items.first, {}, {}, command);

        }

        // See if we can convert children.
        else
        {
            FConvertItem(romfs, std::get<FConversionFileStep>(step.inputFiles[i]), exts, command, convFunc);
        }

    }

}

// Add ZStandard to step.
void FConvertStepZStandard(FRomfs& romfs, FConversionFileStep& step)
{
    ZoneScopedN("FConvertStepZStandard");
    FConvertItem(romfs, step, { ".zs" }, "zstd", [](FRomfs& romfs, const std::string& filePath, const std::string& filePathNoExt)
    {
        DBG_PRINT("CONVERT@LSD: Decompressing \"" << romfs.BasePath(filePath).fullPath << "\".");
        BStreamFile src(romfs.BasePath(filePath), BStreamMode::Read);
        BStreamFile dst(romfs.BasePath(filePathNoExt), BStreamMode::Write);
        ZSTD::Decompress(src, dst, src.Size());
        std::filesystem::remove(romfs.BasePath(filePath).fullPath);
        if (romfs.FileExistsInPatchPath(filePath))
        {
            BStreamFile src(romfs.PatchPath(filePath), BStreamMode::Read);
            BStreamFile dst(romfs.PatchPath(filePathNoExt), BStreamMode::Write);
            ZSTD::Decompress(src, dst, src.Size());
            std::filesystem::remove(romfs.PatchPath(filePath).fullPath);
        }
        return std::make_pair<std::vector<FConversionFileInputMode>, std::vector<FConversionFileInput>>({ FConversionFileInputMode::String }, { filePathNoExt });
    });
}

// Add SARC to step. TODO: RUN SARC RECURSIVELY AND ZSTD!!!
void FConvertStepSARC(FRomfs& romfs, FConversionFileStep& step)
{
    ZoneScopedN("FConvertStepSARC");
    FConvertItem(romfs, step, { ".sarc", ".blarc", ".pack", ".baatarc", ".bkres", ".genvb", ".bfarc"}, "sarc", [](FRomfs& romfs, const std::string& filePath, const std::string& filePathNoExt)
    {
        DBG_PRINT("CONVERT@LSD: Extracting \"" << romfs.BasePath(filePath).fullPath << "\".");
        BStreamFile src(romfs.BasePath(filePath), BStreamMode::Read);
        SARC sarc(src);
        std::filesystem::create_directory(romfs.BasePath(filePathNoExt).fullPath);
        std::vector<FConversionFileInput> inputs;
        std::vector<FConversionFileInputMode> modes;
        for (auto& file : sarc.GetFilenames())
        {
            BStream& src = sarc.Open(file, BStreamMode::Read);
            auto extFile = romfs.BasePath(filePathNoExt + "/" + file);
            std::size_t folderSlashInd = extFile.fullPath.rfind('/');
            if (folderSlashInd != std::string::npos) std::filesystem::create_directories(extFile.fullPath.substr(0, folderSlashInd));
            BStreamFile dst(extFile, BStreamMode::Write);
            dst.Copy(src, src.Size());
            modes.push_back(FConversionFileInputMode::String);
            inputs.push_back(filePathNoExt + "/" + file);
        }
        std::filesystem::remove(romfs.BasePath(filePath).fullPath);
        if (romfs.FileExistsInPatchPath(filePath))
        {
            for (auto& file : sarc.GetFilenames())
            {
                BStream& src = sarc.Open(file, BStreamMode::Read);
                auto extFile = romfs.PatchPath(filePathNoExt + "/" + file);
                std::size_t folderSlashInd = extFile.fullPath.rfind('/');
                if (folderSlashInd != std::string::npos) std::filesystem::create_directories(extFile.fullPath.substr(0, folderSlashInd));
                BStreamFile dst(extFile, BStreamMode::Write);
                dst.Copy(src, src.Size());
            }
            std::filesystem::remove(romfs.PatchPath(filePath).fullPath);
        }
        return std::make_pair<std::vector<FConversionFileInputMode>, std::vector<FConversionFileInput>>(std::move(modes), std::move(inputs));
    });
}

// Add BYML to step.
void FConvertStepBYML(FRomfs& romfs, FConversionFileStep& step)
{
    ZoneScopedN("FConvertStepBYML");
    FConvertItem(romfs, step, { ".byml", ".bgyml", ".byaml" }, "byml", [](FRomfs& romfs, const std::string& filePath, const std::string& filePathNoExt)
    {
        DBG_PRINT("CONVERT@LSD: Deserializing \"" << romfs.BasePath(filePath).fullPath << "\".");
        BStreamFile src(romfs.BasePath(filePath), BStreamMode::Read);
        BYML byml(src);
        int numFiles = byml.WriteYAML(romfs.BasePath(filePathNoExt + ".gyml"));
        std::vector<FConversionFileInputMode> modes = { FConversionFileInputMode::String };
        std::vector<FConversionFileInput> inputs = { filePathNoExt + ".gyml" };
        for (int i = 0; i < numFiles; i++)
        {
            modes.emplace_back(FConversionFileInputMode::String);
            inputs.emplace_back(filePathNoExt + ".gyml." + std::to_string(i));
        }
        std::filesystem::remove(romfs.BasePath(filePath).fullPath);
        if (romfs.FileExistsInPatchPath(filePath))
        {
            BStreamFile src(romfs.PatchPath(filePath), BStreamMode::Read);
            BYML byml(src);
            byml.WriteYAML(romfs.PatchPath(filePathNoExt + ".gyml")); // Minor bug here with no files being used from path and instead base but come on when will this happen.
            std::filesystem::remove(romfs.PatchPath(filePath).fullPath);
        }
        return std::make_pair<std::vector<FConversionFileInputMode>, std::vector<FConversionFileInput>>(std::move(modes), std::move(inputs));
    });
}

// Calculate step info for context. Returns the max step.
std::size_t FConvertCtxUpdate(FConvertProgressCtx& ctx, unsigned int version, const std::string& desc)
{
    ZoneScopedN("FConvertCtxUpdate");
    if (ctx.stepCompleted)
    {
        ctx.currStep = 0;
        ctx.numSteps = ctx.file->steps.size();
        ctx.version = version;
        ctx.convStepDesc = desc;
    }
    std::size_t max = std::min(ctx.numSteps, ctx.currStep + MAX_NUM_STEPS);
    ctx.stepCompleted = max == ctx.numSteps;
    ctx.full = ctx.numSteps - ctx.currStep >= MAX_NUM_STEPS;
    return max;
}

// Version 1 - ZStandard (de)compression.
void FConvertVersion1(FRomfs& romfs, FConvertProgressCtx& ctx)
{
    ZoneScopedN("FConvertVersion1");
    std::size_t max = FConvertCtxUpdate(ctx, 0, "Decompressing files...");
    for (std::size_t i = ctx.currStep; i < max; i++)
    {
        FConvertStepZStandard(romfs, ctx.file->steps[i]);
    }
    ctx.currStep = max;
}

// Version 2 - SARC extraction. TODO: HANDLE RECURSIVE FILES!!!
void FConvertVersion2(FRomfs& romfs, FConvertProgressCtx& ctx)
{
    ZoneScopedN("FConvertVersion2");
    std::size_t max = FConvertCtxUpdate(ctx, 1, "Extracting archives...");
    for (std::size_t i = ctx.currStep; i < max; i++)
    {
        FConvertStepSARC(romfs, ctx.file->steps[i]);
    }
    ctx.currStep = max;
}

// Version 3 - Deserialize BYML.
void FConvertVersion3(FRomfs& romfs, FConvertProgressCtx& ctx)
{
    ZoneScopedN("FConvertVersion3");
    std::size_t max = FConvertCtxUpdate(ctx, 2, "Deserializing YAML...");
    for (std::size_t i = ctx.currStep; i < max; i++)
    {
        FConvertStepBYML(romfs, ctx.file->steps[i]);
    }
    ctx.currStep = max;
}

bool FConvert::UpdateRomfs(FRomfs& romfs, FConvertProgressCtx& ctx, bool forceReFileChecks)
{
    ZoneScopedN("FConvert::UpdateRomfs");
    if (!ctx.file) ctx.file = JPtrMake(FConversionFile, romfs, false);
    if (ctx.file->version == VERSION_INVALID)
    {
        DBG_PRINT("ROMFS@LSD: Can not mix and match base and patch versions!");
        ctx.file = nullptr;
        return false;
    }
    if (ctx.version < 1 && (forceReFileChecks || ctx.file->version < 1)) FConvertVersion1(romfs, ctx);
    if (!ctx.full && ctx.version < 2 && (forceReFileChecks || ctx.file->version < 2)) FConvertVersion2(romfs, ctx);
    if (!ctx.full && ctx.version < 3 && (forceReFileChecks || ctx.file->version < 3)) FConvertVersion3(romfs, ctx);
    if (!ctx.full)
    {
        ctx.file->version = 3;
        ctx.file->Save(romfs);
    }
    ctx.full = !ctx.full; // Signal we are done if we have nothing left to convert, otherwise make sure user knows we are not done.
    return true;
}

void FConvert::CompileRomfs(const FRomfs& romfs, const JResPath& dstDir)
{
    ZoneScopedN("FConvert::CompileRomfs");
}