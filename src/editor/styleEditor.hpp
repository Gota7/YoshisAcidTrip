#pragma once

#include "style.hpp"
#include <map>

// Forward declare.
class EEditor;
class ESettings;

// Editor for styles.
class EStyleEditor
{
    EEditor& editor;
    ESettings& settings;
    std::vector<std::string> themes;
    int currTheme = 0;
    bool needsScan = false;
    bool open = true;

    // Scan for themes.
    void Scan();

    // Get theme by index.
    static bool GetTheme(void* data, int idx, const char** outText);

    // Get font by index.
    static bool GetFont(void* data, int idx, const char** outText);

public:

    // Make a new style editor. Usually a singleton for the editor.
    EStyleEditor(EEditor& editor, ESettings& settings);

    // Get full path for theme name.
    static JResPath ThemePath(const std::string& name);

    // Draw the GUI.
    void DrawUI();

};