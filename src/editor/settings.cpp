#include "settings.hpp"

/*

    Settings file changelog:

        0.0.0: Version
        0.0.1: Added theme name, mods, default mod.

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
    root << YAML::Key << "Version" << YAML::Value << YAML::Flow << YAML::BeginSeq << VERSION[VER_INDEX_MAJOR] << VERSION[VER_INDEX_MINOR] << VERSION[VER_INDEX_REV] << YAML::EndSeq;
    YAML::KeyValue(root, "Theme", currTheme);
    YAML::KeyValue(root, "Mod", currMod);
    root << YAML::Key << "Mods" << YAML::Value << YAML::BeginMap;
    for (auto& mod : mods)
    {
        mod.second.Save(root, mod.first);
    }
    root << YAML::EndMap;
}