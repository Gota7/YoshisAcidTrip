#include <texture/formats/common.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TFormatCommon::TFormatCommon(const JResPath& path)
{
    ZoneScopedN("TFormatCommon::TFormatCommon");
    texData = stbi_load(path.fullPath.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
    if (!texData) std::cout << "TEXTURE@JUNO: Failed to load texture \"" << path.fullPath << "\"!" << std::endl;
}

void TFormatCommon::Properties(int& width, int& height, int& numChannels)
{
    ZoneScopedN("TFormatCommon::Properties");
    width = this->width;
    height = this->height;
    numChannels = this->numChannels;
}

const void* TFormatCommon::DataPtr()
{
    ZoneScopedN("TFormatCommon::DataPtr");
    return texData;
}

TFormatCommon::~TFormatCommon()
{
    ZoneScopedN("TFormatCommon::~TFormatCommon");
    stbi_image_free(texData);
}