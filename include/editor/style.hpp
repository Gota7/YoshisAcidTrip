#pragma once

#include <jsystem/fs.hpp>
#include <imgui.h>

// Editor style.
class EStyle
{
    ImGuiStyle style;
public:

    // Friends :>
    friend class EStyleEditor;

    // Load a style from a file.
    EStyle(const JResPath& path);

    // Save the style to a file.
    void Save(const JResPath& path);

    // Set as the current style.
    void Set();

};

extern const JResPath STYLE_DEFAULT;