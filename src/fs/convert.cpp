#include "convert.hpp"

#include "../bin/formats/sarc.hpp"
#include "../bin/formats/zstd.hpp"
#include "conversionFile.hpp"
#include <functional>

// Using an output file and one with extension removed, debinarize into input files.
using FConvertItemFunc = std::function<std::pair<std::vector<FConversionFileInputMode>, std::vector<FConversionFileInput>>(FRomfs&, const std::string&, const std::string&)>;

inline bool FConvertEndsWith(std::string const& value, std::string const& ending)
{
    ZoneScopedN("FConvertEndsWith");
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::optional<FRomfs> FConvert::MakeEditorRomfs(const JResPath& srcDir, const JResPath& baseDir, const JResPath& patchDir)
{
    ZoneScopedN("FConvert::MakeEditorRomfs");

    // Make sure source exists.
    if (!std::filesystem::exists(srcDir.fullPath)) return std::nullopt;

    // Make directories.
    std::filesystem::create_directories(baseDir.fullPath);
    std::filesystem::create_directories(patchDir.fullPath);
    std::filesystem::copy(srcDir.fullPath, baseDir.fullPath, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);

    // Add initial version.
    std::optional<FRomfs> ret(FRomfs(baseDir, patchDir));
    FConversionFile file(*ret, true);
    file.Save(*ret);
    UpdateRomfs(*ret);
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

// Add SARC to step.
void FConvertStepSARC(FRomfs& romfs, FConversionFileStep& step)
{
    ZoneScopedN("FConvertStepSARC");
    FConvertItem(romfs, step, { ".sarc", ".blarc", ".pack", ".baatarc" }, "sarc", [](FRomfs& romfs, const std::string& filePath, const std::string& filePathNoExt)
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

// Version 1 - ZStandard (de)compression.
void FConvertVersion1(FRomfs& romfs, FConversionFile& convFile)
{
    ZoneScopedN("FConvertVersion1");
    for (auto& step : convFile.steps)
        FConvertStepZStandard(romfs, step);
}

// Version 2 - SARC extraction.
void FConvertVersion2(FRomfs& romfs, FConversionFile& convFile)
{
    ZoneScopedN("FConvertVersion2");
    for (auto& step : convFile.steps)
        FConvertStepSARC(romfs, step);
    FConvertVersion1(romfs, convFile); // Important to make sure ZStandard files in the SARC are decompressed too.
}

bool FConvert::UpdateRomfs(FRomfs& romfs)
{
    ZoneScopedN("FConvert::UpdateRomfs");
    FConversionFile convFile(romfs, false);
    if (convFile.version == VERSION_INVALID)
    {
        DBG_PRINT("ROMFS@LSD: Can not mix and match base and patch versions!");
        return false;
    }
    if (convFile.version < 1) FConvertVersion1(romfs, convFile);
    if (convFile.version < 2) FConvertVersion2(romfs, convFile);
    convFile.version = 2;
    convFile.Save(romfs);
    return true;
}

void FConvert::CompileRomfs(const FRomfs& romfs, const JResPath& dstDir)
{
    ZoneScopedN("FConvert::CompileRomfs");
}