#pragma once

#include <jsystem/fs.hpp>
#include <imgui.h>

// Forward declare.
class JWindow;

// Editor style.
class EStyle
{
    ImGuiStyle style;
    std::string font = "Default";
    float fontSize = 12.0f;
public:

    // Friends :>
    friend class EStyleEditor;

    // Load a style from a file.
    EStyle(const JResPath& path);

    // Save the style to a file.
    void Save(const JResPath& path);

    // Set as the current style.
    void Set(JWindow& window);

};

extern const JResPath STYLE_DEFAULT;