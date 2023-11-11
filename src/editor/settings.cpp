#include "settings.hpp"

#include "editor.hpp"

/*

    Settings file changelog:

        0.0.0: Version
        0.0.1: Added lang, theme name, mods, default mod.

*/

void ESettingsMod::Load(YAML::Node& node)
{
    ZoneScopedN("ESettingsMod::Load");
}

void ESettingsMod::Save(YAML::Emitter& node, const std::string& name)
{
    ZoneScopedN("ESettingsMod::Save");
    node << YAML::Key << name << YAML::Value << YAML::BeginMap;
    YAML::KeyValue(node, "Base", basePath);
    YAML::KeyValue(node, "Build", buildPath);
    YAML::KeyValue(node, "Patch", patchPath);
    YAML::KeyValue(node, "LastLevel", lastLevel);
    node << YAML::EndMap;
}

int ESettings::CompareVer(const EVersion& a, const EVersion& b)
{
    ZoneScopedN("ESettings::CompareVer");
    for (unsigned int i = 0; i < 3; i++)
    {
        if (a[i] < b[i]) return -1;
        else if (a[i] > b[i]) return 1;
    }
    return 0;
}

void ESettings::Load(JWindow& window)
{
    ZoneScopedN("ESettings::Load");
    if (JFileSystem::FileExists(JResPath("settings.yaml")))
    {
        YAML::Node root = YAML::LoadFile(JResPath("settings.yaml").fullPath);
        auto ver = root["Version"].as<EVersion>();
        if (CompareVer(ver, { 0, 0, 1 }) >= 0)
        {

            // Lang.
            lang = root["Lang"].as<std::string>();

            // Theme.
            currTheme = root["Theme"].as<std::string>();
            currStyle = JPtrMake(EStyle, EStyleEditor::ThemePath(currTheme));
            currStyle->Set(window);

            // Mod.
            currMod = root["Mod"].as<std::string>();
            YAML::Node mods = root["Mods"];
            // for (auto& mod : mods)
            // {
            //     mod.
            // }

        }
    }
}

void ESettings::Save()
{
    ZoneScopedN("ESettings::Save");
    auto file = JFileSystem::SaveFile(JResPath("settings.yaml"));
    YAML::Emitter root(file);
    root << YAML::BeginMap << YAML::Key << "Version" << YAML::Value << YAML::Flow << YAML::BeginSeq << VERSION[VER_INDEX_MAJOR] << VERSION[VER_INDEX_MINOR] << VERSION[VER_INDEX_REV] << YAML::EndSeq;
    YAML::KeyValue(root, "Lang", lang);
    YAML::KeyValue(root, "Theme", currTheme);
    YAML::KeyValue(root, "Mod", currMod);
    root << YAML::Key << "Mods" << YAML::Value << YAML::BeginMap;
    for (auto& mod : mods)
    {
        mod.second.Save(root, mod.first);
    }
    root << YAML::EndMap << YAML::EndMap;
}

void ESettings::ReloadLangList()
{
    ZoneScopedN("ESettings::ReloadLangList");

    // Alphabetize langs and get current theme index.
    langs.clear();
    currLangInd = 0;
    for (auto& file : std::filesystem::directory_iterator(std::filesystem::path(JResPath("lng").fullPath)))
    {
        if (!file.is_directory())
        {
            langs.push_back(file.path().filename().replace_extension("").string());
        }
    }
    std::sort(langs.begin(), langs.end());
    int ind = 0;
    for (auto& lang : langs)
    {
        if (lang == this->lang)
        {
            currLangInd = ind;
            break;
        }
        ind++;
    }

}

bool ESettings::GetLang(void* data, int idx, const char** outText)
{
    ZoneScopedN("ESettings::GetLang");
    ESettings* ed = (ESettings*)data;
    if ((std::size_t)idx >= ed->langs.size()) return false;
    else *outText = ed->langs[idx].c_str();
    return true;
}

JResPath ESettings::LangPath(const std::string& name)
{
    ZoneScopedN("ESettings::LangPath");
    return JResPath(JFileSystem::PathSeparators(JResPath("lng").fullPath + "/" + name + ".yaml"), true);
}

bool ESettings::DrawLangSelector(EEditor& editor)
{
    ZoneScopedN("ESettings::DrawLangSelector");
    if (ImGui::Combo(editor.text["Language"].c_str(), &currLangInd, GetLang, this, (int)langs.size()))
    {
        lang = langs[currLangInd];
        YAML::Node node = YAML::LoadFile(JResPath("lng/" + lang + ".yaml").fullPath);
        editor.text = node.as<std::map<std::string, std::string>>();
        return true;
    }
    return false;
}

void ESettings::ReloadThemeList()
{
    ZoneScopedN("ESettings::ReloadThemeList");

    // Alphabetize themes and get current theme index.
    themes.clear();
    currThemeInd = 0;
    for (auto& file : std::filesystem::directory_iterator(std::filesystem::path(JResPath("thm").fullPath)))
    {
        if (!file.is_directory())
        {
            themes.push_back(file.path().filename().replace_extension("").string());
        }
    }
    std::sort(themes.begin(), themes.end());
    int ind = 0;
    for (auto& theme : themes)
    {
        if (theme == currTheme)
        {
            currThemeInd = ind;
            break;
        }
        ind++;
    }

}

bool ESettings::GetTheme(void* data, int idx, const char** outText)
{
    ZoneScopedN("ESettings::GetTheme");
    ESettings* ed = (ESettings*)data;
    if ((std::size_t)idx >= ed->themes.size()) return false;
    else *outText = ed->themes[idx].c_str();
    return true;
}

JResPath ESettings::ThemePath(const std::string& name)
{
    ZoneScopedN("ESettings::ThemePath");
    return JResPath(JFileSystem::PathSeparators(JResPath("thm").fullPath + "/" + name + ".ini"), true);
}

bool ESettings::DrawThemeSelector(EEditor& editor)
{
    ZoneScopedN("ESettings::DrawThemeSelector");
    if (ImGui::Combo(editor.text["Style"].c_str(), &currThemeInd, GetTheme, this, (int)themes.size()))
    {
        currStyle = JPtrMake(EStyle, ThemePath(themes[currThemeInd]));
        currStyle->Set(editor.window);
        currTheme = themes[currThemeInd];
        Save();
        return true;
    }
    return false;
}