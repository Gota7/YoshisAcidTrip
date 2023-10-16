#pragma once

#include "../jsystem/fs.hpp"
#include "formats/common.hpp"
#include "format.hpp"
#include <map>

// Texture loader.
namespace TLoader
{
    inline std::map<std::string, TTextureLoader> Formats =
    {
        { ".bmp", TFormatCommon::Load },
        { ".gif", TFormatCommon::Load },
        { ".hdr", TFormatCommon::Load },
        { ".jpg", TFormatCommon::Load },
        // { ".jxl", TFormatJpegXL::Load },
        { ".pic", TFormatCommon::Load },
        { ".png", TFormatCommon::Load },
        { ".pnm", TFormatCommon::Load },
        { ".psd", TFormatCommon::Load },
        { ".tga", TFormatCommon::Load }
    };

    // Load a texture.
    JPtr<TFormat> LoadTexture(const JResPath& path);

};