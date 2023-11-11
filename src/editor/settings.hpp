#pragma once

#include "../jsystem/mem.hpp"
#include "../yaml/helper.hpp"
#include "style.hpp"
#include "styleEditor.hpp"
#include <imgui.h>

#define VER_INDEX_MAJOR 0
#define VER_INDEX_MINOR 1
#define VER_INDEX_REV 2

// Version type.
using EVersion = std::array<uint32_t, 3>;

// Forward declare.
class EEditor;
class JWindow;

// Editor settings mod.
class ESettingsMod
{
public:
    std::string basePath;
    std::string buildPath;
    std::string patchPath;
    std::string lastLevel = "";

    // Load from YAML.
    void Load(YAML::Node& node);

    // Save to YAML.
    void Save(YAML::Emitter& node, const std::string& name);

};

// Editor settings.
class ESettings
{
    std::vector<std::string> langs;
    std::vector<std::string> themes;
    int currThemeInd;
    int currLangInd;

    // Get a lang.
    static bool GetLang(void* data, int idx, const char** outText);

    // Get a lang path.
    static JResPath LangPath(const std::string& name);

    // Get a theme.
    static bool GetTheme(void* data, int idx, const char** outText);

    // Get a theme path.
    static JResPath ThemePath(const std::string& name);

public:
    const EVersion VERSION = { 0, 0, 1 };
    std::map<std::string, ESettingsMod> mods;
    std::string currMod;
    std::string currTheme = "Wonder";
    std::string lang = "English";
    JPtr<EStyle> currStyle;

    // Make new settings.
    ESettings(JWindow& window) : currStyle(JPtrMake(EStyle, EStyleEditor::ThemePath(currTheme)))
    {
        currStyle->Set(window);
    }

    // Compare versions. Returns negative if a < b, zero if a == b, and positive if a > b.
    static int CompareVer(const EVersion& a, const EVersion& b);

    // Load settings from file.
    void Load(JWindow& window);

    // Save settings.
    void Save();

    // Reload language list.
    void ReloadLangList();

    // Draw language selector.
    bool DrawLangSelector(EEditor& editor);

    // Reload theme list.
    void ReloadThemeList();

    // Draw theme selector.
    bool DrawThemeSelector(EEditor& editor);

};