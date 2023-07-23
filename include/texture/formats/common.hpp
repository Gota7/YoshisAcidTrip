#pragma once

#include <texture/format.hpp>
#include <stb_image.h>

// Common texture formats.
class TFormatCommon : public TFormat
{
private:
    stbi_uc* texData; // Texture data.
    int width; // Width of the texture.
    int height; // Height of the texture.
    int numChannels; // How many channels the texture has.

public:

    // Make a common format.
    TFormatCommon(const JResPath& path);

    // Load a common format.
    static JPtr<TFormat> Load(const JResPath& path)
    {
        return JPtrMake(TFormatCommon, path);
    }

    // Vfunctions.
    virtual void Properties(int& width, int& height, int& numChannels) const override;
    virtual const void* DataPtr() const override;
    virtual ~TFormatCommon() override;

};