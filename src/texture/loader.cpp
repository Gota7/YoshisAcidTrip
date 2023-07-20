#include <texture/loader.hpp>

JPtr<TFormat> TLoader::LoadTexture(const JResPath& path)
{
    ZoneScopedN("TLoader::LoadTexture");

    // Get extension.
    TTextureLoader loader = nullptr;
    std::filesystem::path pt(path.fullPath);
    auto res = Formats.find(pt.extension().string());
    if (res != Formats.end())
    {
        loader = res->second;
        DBG_PRINT("TEXTURE@JUNO: Loading \"" << path.fullPath << "\".");
        return loader(path);
    }
    else
    {
        DBG_PRINT("TEXTURE@JUNO: Can not determine file type of file \"" << path.fullPath << "\".");
        return nullptr;
    }

}