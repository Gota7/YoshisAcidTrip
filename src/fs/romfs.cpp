#include "romfs.hpp"

FRomfs::FRomfs(const JResPath& baseDir, const JResPath& patchDir) : baseDir(baseDir), patchDir(patchDir)
{
    ZoneScopedN("FRomfs::FRomfs");
}

JResPath FRomfs::BasePath(const std::string& path)
{
    ZoneScopedN("FRomfs::BasePath");
    return JResPath(baseDir.fullPath + "/" + path, true);
}

JResPath FRomfs::PatchPath(const std::string& path)
{
    ZoneScopedN("FRomfs::PatchPath");
    return JResPath(patchDir.fullPath + "/" + path, true);
}

bool FRomfs::FileExistsInPatchPath(const std::string& path)
{
    ZoneScopedN("FRomfs::FileExistsInPatchPath");
    return JFileSystem::FileExists(PatchPath(path));
}

bool FRomfs::FileExists(const std::string& path)
{
    ZoneScopedN("FRomfs::FileExists");
    if (FileExistsInPatchPath(path)) return true;
    else return JFileSystem::FileExists(BasePath(path));
}

BStreamFile FRomfs::Open(const std::string& path, bool write)
{
    ZoneScopedN("FRomfs::Open");
    if (write || FileExistsInPatchPath(path))
    {
        return BStreamFile(PatchPath(path), write ? BStreamMode::Write : BStreamMode::Read);
    }
    else
    {
        if (!FileExists(path))
        {
            DBG_PRINT("ROMFS@LSD: Tried to open non-existent file \"" << path << "\".");
        }
        return BStreamFile(BasePath(path), BStreamMode::Read);
    }
}