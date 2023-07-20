#include <texture/formats/common.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TFormatCommon::TFormatCommon(const JResPath& path)
{
    ZoneScopedN("TFormatCommon::TFormatCommon");
    texData = stbi_load(path.fullPath.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
    if (!texData) DBG_PRINT("TEXTURE@JUNO: Failed to load texture \"" << path.fullPath << "\"!");
}

void TFormatCommon::Properties(int& width, int& height, int& numChannels) const
{
    ZoneScopedN("TFormatCommon::Properties");
    width = this->width;
    height = this->height;
    numChannels = this->numChannels;
}

const void* TFormatCommon::DataPtr() const
{
    ZoneScopedN("TFormatCommon::DataPtr");
    return texData;
}

TFormatCommon::~TFormatCommon()
{
    ZoneScopedN("TFormatCommon::~TFormatCommon");
    stbi_image_free(texData);
}