#pragma once

#include <imgui.h>
#include <jsystem/fs.hpp>

// Editor style.
class EStyle
{
    ImGuiStyle style;
public:
    const JResPath DEFAULT = JResPath("default");

    // Load a style from a file.
    EStyle(const JResPath& path);

    // Save the style to a file.
    void Save(const JResPath& path);

    // Set as the current style.
    void Set();

};