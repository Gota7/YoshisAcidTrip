#pragma once

#include "../jsystem/mem.hpp"
#include "editor/style.hpp"
#include "editor/styleEditor.hpp"
#include <imgui.h>

// Forward declare.
class JWindow;

// Editor settings.
class ESettings
{
public:
    const ImVec4 VERSION = ImVec4(0.0f, 0.0f, 1.0f, 0.0f);
    std::string currTheme = "Wonder";
    JPtr<EStyle> currStyle;

    // Make new settings.
    ESettings(JWindow& window) : currStyle(JPtrMake(EStyle, EStyleEditor::ThemePath(currTheme)))
    {
        currStyle->Set(window);
    }

    // Save settings. TODO!!!
    void Save() {}

};