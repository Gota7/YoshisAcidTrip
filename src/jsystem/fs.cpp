#include <jsystem/fs.hpp>
#include <filesystem>
#include <regex>

#if defined(_WIN32)
    #include <windows.h>
    #include <shlwapi.h>
    #include <io.h>

    #define access _access_s
#endif

#ifdef __APPLE__
    #include <libgen.h>
    #include <limits.h>
    #include <mach-o/dyld.h>
    #include <unistd.h>
#endif

#ifdef __linux__
    #include <limits.h>
    #include <libgen.h>
    #include <unistd.h>

    #if defined(__sun)
        #define PROC_SELF_EXE "/proc/self/path/a.out"
    #else
        #define PROC_SELF_EXE "/proc/self/exe"
    #endif

#endif

JResPath::JResPath(const std::string& relPath, bool force) : fullPath(force ? relPath : JFileSystem::AbsPath(relPath))
{
    ZoneScopedN("JResPath::JResPath");
}

#if defined(__linux__) || defined(__APPLE__)

std::string JFileSystem::PathSeparators(const std::string& path)
{
    ZoneScopedN("JFileSystem::PathSeparators");
    return path; // Linux and Unix use the same path separators.
}

#elif defined(_WIN32)

std::string JFileSystem::PathSeparators(const std::string& path)
{
    ZoneScopedN("JFileSystem::PathSeparators");
    std::string ret = path;
    for (std::size_t i = 0; i < ret.size(); i++)
    {
        if (ret.at(i) == '/') ret.at(i) = '\\';
    }
    return ret;
}

#endif

#ifdef __DEBUG__

std::string JFileSystem::AbsPath(const std::string& path)
{
    ZoneScopedN("JFileSystem::AbsPath");
    return PathSeparators("res/" + path); // Use local res folder.
}

#elif defined(__linux__)

std::string JFileSystem::AbsPath(const std::string& path)
{
    ZoneScopedN("JFileSystem::AbsPath");
    char rawPathName[PATH_MAX];
    realpath(PROC_SELF_EXE, rawPathName);
    char* executableDir = dirname(rawPathName);
    return std::string(executableDir) + "/res/" + PathSeparators(path);
}

#elif defined(__APPLE__)

std::string JFileSystem::AbsPath(const std::string& path)
{
    ZoneScopedN("JFileSystem::AbsPath");
    char rawPathName[PATH_MAX];
    char realPathName[PATH_MAX];
    uint32_t rawPathSize = (uint32_t)sizeof(rawPathName);
    if (!_NSGetExecutablePath(rawPathName, &rawPathSize))
    {
        realpath(rawPathName, realPathName);
    }
    char* executableDir = dirname(realPathName);
    return std::string(executableDir) + "/res/" + PathSeparators(path);
}

#elif defined(_WIN32)

std::string JFileSystem::AbsPath(const std::string& path)
{
    ZoneScopedN("JFileSystem::AbsPath");
    char rawPathName[MAX_PATH];
    GetModuleFileNameA(NULL, rawPathName, MAX_PATH);
    return std::filesystem::path(std::string(rawPathName)).parent_path().string() + "\\res\\" + PathSeparators(path);
}

#endif