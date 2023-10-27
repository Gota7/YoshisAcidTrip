#pragma once

#include "../bin/streams/file.hpp"
#include "../jsystem/fs.hpp"

// Represents a ROMFS.
class FRomfs
{
    JResPath baseDir;
    JResPath patchDir;

public:

    // Friends :}
    friend class FConversionFile;

    // Open a ROMFS. Note that it must of been pre-converted by the editor.
    FRomfs(const JResPath& baseDir, const JResPath& patchDir);

    // Get full path for file in base path.
    JResPath BasePath(const std::string& path);

    // Get full path for file in patch path.
    JResPath PatchPath(const std::string& path);

    // If file exists in patch path.
    bool FileExistsInPatchPath(const std::string& path);

    // If a file exists.
    bool FileExists(const std::string& path);

    // Open a file.
    BStreamFile Open(const std::string& path, bool write = false);

};