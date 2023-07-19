#include <texture/loader.hpp>

#include <iostream>

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
        std::cout << "TEXTURE@JUNO: Loading \"" << path.fullPath << "\"." << std::endl;
        return loader(path);
    }
    else
    {
        std::cout << "TEXTURE@JUNO: Can not determine file type of file \"" << path.fullPath << "\"." << std::endl;
        return nullptr;
    }

}