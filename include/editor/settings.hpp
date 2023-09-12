#pragma once

#include <editor/style.hpp>
#include <editor/styleEditor.hpp>
#include <jsystem/mem.hpp>
#include <imgui.h>

// Editor settings.
class ESettings
{
public:
    const ImVec4 VERSION = ImVec4(0.0f, 0.0f, 1.0f, 0.0f);
    std::string currTheme = "Cat & Frog";
    JPtr<EStyle> currStyle;

    // Make new settings.
    ESettings() : currStyle(JPtrMake(EStyle, EStyleEditor::ThemePath(currTheme)))
    {
        currStyle->Set();
    }

    // Save settings. TODO!!!
    void Save() {}

};