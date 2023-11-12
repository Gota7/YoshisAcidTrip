#include "style.hpp"

#include "../jsystem/window.hpp"
#include "../yaml/helper.hpp"
#include <iostream>

const JResPath STYLE_DEFAULT = JResPath("default");

EStyle::EStyle(const JResPath& path)
{
    ZoneScopedN("EStyle::EStyle");
    if (path.fullPath == STYLE_DEFAULT.fullPath || !JFileSystem::FileExists(path))
    {
        ImGui::StyleColorsClassic(&style);
        return;
    }
    YAML::Node t = YAML::LoadFile(path.fullPath);
    font = t["Font"]["Name"].as<std::string>();
    if (!JFileSystem::FileExists(JFileSystem::PathSeparators("fnt/" + font + ".ttf")))
    {
        font = "Default";
    }
    fontSize = t["Font"]["Size"].as<float>();
    style.Alpha = t["Theme"]["Alpha"].as<float>();
    style.AntiAliasedFill = t["Theme"]["AntiAliasedFill"].as<bool>();
    style.AntiAliasedLines = t["Theme"]["AntiAliasedLines"].as<bool>();
    style.AntiAliasedLinesUseTex = t["Theme"]["AntiAliasedLinesUseTex"].as<bool>();
    style.ButtonTextAlign.x = t["Theme"]["ButtonTextAlignX"].as<float>();
    style.ButtonTextAlign.y = t["Theme"]["ButtonTextAlignY"].as<float>();
    style.CellPadding.x = t["Theme"]["CellPaddingX"].as<float>();
    style.CellPadding.y = t["Theme"]["CellPaddingY"].as<float>();
    style.ChildBorderSize = t["Theme"]["ChildBorderSize"].as<float>();
    style.ChildRounding = t["Theme"]["ChildRounding"].as<float>();
    style.CircleTessellationMaxError = t["Theme"]["CircleTessellationMaxError"].as<float>();
    style.ColorButtonPosition = (ImGuiDir)t["Theme"]["ColorButtonPosition"].as<int>();
    for (int i = 0; i < ImGuiCol_COUNT; i++)
    {
        std::string key = ImGui::GetStyleColorName(i);
        if (t["Theme"]["Colors"][key].IsDefined())
        {
            style.Colors[i].x = t["Theme"]["Colors"][key]["R"].as<float>();
            style.Colors[i].y = t["Theme"]["Colors"][key]["G"].as<float>();
            style.Colors[i].z = t["Theme"]["Colors"][key]["B"].as<float>();
            style.Colors[i].w = t["Theme"]["Colors"][key]["A"].as<float>();
        }
    }
    style.ColumnsMinSpacing = t["Theme"]["ColumnsMinSpacing"].as<float>();
    style.CurveTessellationTol = t["Theme"]["CurveTessellationTol"].as<float>();
    style.DisplaySafeAreaPadding.x = t["Theme"]["DisplaySafeAreaPaddingX"].as<float>();
    style.DisplaySafeAreaPadding.y = t["Theme"]["DisplaySafeAreaPaddingY"].as<float>();
    style.DisplayWindowPadding.x = t["Theme"]["DisplayWindowPaddingX"].as<float>();
    style.DisplayWindowPadding.y = t["Theme"]["DisplayWindowPaddingY"].as<float>();
    style.FrameBorderSize = t["Theme"]["FrameBorderSize"].as<float>();
    style.FramePadding.x = t["Theme"]["FramePaddingX"].as<float>();
    style.FramePadding.y = t["Theme"]["FramePaddingY"].as<float>();
    style.FrameRounding = t["Theme"]["FrameRounding"].as<float>();
    style.GrabMinSize = t["Theme"]["GrabMinSize"].as<float>();
    style.GrabRounding = t["Theme"]["GrabRounding"].as<float>();
    style.IndentSpacing = t["Theme"]["IndentSpacing"].as<float>();
    style.ItemInnerSpacing.x = t["Theme"]["ItemInnerSpacingX"].as<float>();
    style.ItemInnerSpacing.y = t["Theme"]["ItemInnerSpacingY"].as<float>();
    style.ItemSpacing.x = t["Theme"]["ItemSpacingX"].as<float>();
    style.ItemSpacing.y = t["Theme"]["ItemSpacingY"].as<float>();
    style.LogSliderDeadzone = t["Theme"]["LogSliderDeadzone"].as<float>();
    style.MouseCursorScale = t["Theme"]["MouseCursorScale"].as<float>();
    style.PopupBorderSize = t["Theme"]["PopupBorderSize"].as<float>();
    style.PopupRounding = t["Theme"]["PopupRounding"].as<float>();
    style.ScrollbarRounding = t["Theme"]["ScrollbarRounding"].as<float>();
    style.ScrollbarSize = t["Theme"]["ScrollbarSize"].as<float>();
    style.SelectableTextAlign.x = t["Theme"]["SelectableTextAlignX"].as<float>();
    style.SelectableTextAlign.y = t["Theme"]["SelectableTextAlignY"].as<float>();
    style.TabBorderSize = t["Theme"]["TabBorderSize"].as<float>();
    style.TabMinWidthForCloseButton = t["Theme"]["TabMinWidthForCloseButton"].as<float>();
    style.TabRounding = t["Theme"]["TabRounding"].as<float>();
    style.TouchExtraPadding.x = t["Theme"]["TouchExtraPaddingX"].as<float>();
    style.TouchExtraPadding.y = t["Theme"]["TouchExtraPaddingY"].as<float>();
    style.WindowBorderSize = t["Theme"]["WindowBorderSize"].as<float>();
    style.WindowMenuButtonPosition = (ImGuiDir)t["Theme"]["WindowMenuButtonPosition"].as<int>();
    style.WindowMinSize.x = t["Theme"]["WindowMinSizeX"].as<float>();
    style.WindowMinSize.y = t["Theme"]["WindowMinSizeY"].as<float>();
    style.WindowPadding.x = t["Theme"]["WindowPaddingX"].as<float>();
    style.WindowPadding.y = t["Theme"]["WindowPaddingY"].as<float>();
    style.WindowRounding = t["Theme"]["WindowRounding"].as<float>();
    style.WindowTitleAlign.x = t["Theme"]["WindowTitleAlignX"].as<float>();
    style.WindowTitleAlign.y = t["Theme"]["WindowTitleAlignY"].as<float>();
}

float EStyle::FontSize() const
{
    ZoneScopedN("EStyle::FontSize");
    return fontSize;
}

void EStyle::Save(const JResPath& path)
{
    ZoneScopedN("EStyle::Save");
    if (path.fullPath == STYLE_DEFAULT.fullPath) return;
    auto yamlFile = JFileSystem::SaveFile(path);
    YAML::Emitter node(yamlFile);
    node << YAML::BeginMap << YAML::Key << "Font" << YAML::Value << YAML::BeginMap;
    YAML::KeyValue(node, "Name", font);
    YAML::KeyValue(node, "Size", fontSize);
    node << YAML::EndMap << YAML::Key << "Theme" << YAML::Value << YAML::BeginMap;
    YAML::KeyValue(node, "Alpha", style.Alpha);
    YAML::KeyValue(node, "AntiAliasedFill", style.AntiAliasedFill);
    YAML::KeyValue(node, "AntiAliasedLines", style.AntiAliasedLines);
    YAML::KeyValue(node, "AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex);
    YAML::KeyValue(node, "ButtonTextAlignX", style.ButtonTextAlign.x);
    YAML::KeyValue(node, "ButtonTextAlignY", style.ButtonTextAlign.y);
    YAML::KeyValue(node, "CellPaddingX", style.CellPadding.x);
    YAML::KeyValue(node, "CellPaddingY", style.CellPadding.y);
    YAML::KeyValue(node, "ChildBorderSize", style.ChildBorderSize);
    YAML::KeyValue(node, "ChildRounding", style.ChildRounding);
    YAML::KeyValue(node, "CircleTessellationMaxError", style.CircleTessellationMaxError);
    YAML::KeyValue(node, "ColorButtonPosition", (int)style.ColorButtonPosition);
    node << YAML::Key << "Colors" << YAML::Value << YAML::BeginMap;
    for (int i = 0; i < ImGuiCol_COUNT; i++)
    {
        node << YAML::Key << ImGui::GetStyleColorName(i) << YAML::Value << YAML::BeginMap;
        YAML::KeyValue(node, "R", style.Colors[i].x);
        YAML::KeyValue(node, "G", style.Colors[i].y);
        YAML::KeyValue(node, "B", style.Colors[i].z);
        YAML::KeyValue(node, "A", style.Colors[i].w);
        node << YAML::EndMap;
    }
    node << YAML::EndMap;
    YAML::KeyValue(node, "ColumnsMinSpacing", style.ColumnsMinSpacing);
    YAML::KeyValue(node, "CurveTessellationTol", style.CurveTessellationTol);
    YAML::KeyValue(node, "DisplaySafeAreaPaddingX", style.DisplaySafeAreaPadding.x);
    YAML::KeyValue(node, "DisplaySafeAreaPaddingY", style.DisplaySafeAreaPadding.y);
    YAML::KeyValue(node, "DisplayWindowPaddingX", style.DisplayWindowPadding.x);
    YAML::KeyValue(node, "DisplayWindowPaddingY", style.DisplayWindowPadding.y);
    YAML::KeyValue(node, "FrameBorderSize", style.FrameBorderSize);
    YAML::KeyValue(node, "FramePaddingX", style.FramePadding.x);
    YAML::KeyValue(node, "FramePaddingY", style.FramePadding.y);
    YAML::KeyValue(node, "FrameRounding", style.FrameRounding);
    YAML::KeyValue(node, "GrabMinSize", style.GrabMinSize);
    YAML::KeyValue(node, "GrabRounding", style.GrabRounding);
    YAML::KeyValue(node, "IndentSpacing", style.IndentSpacing);
    YAML::KeyValue(node, "ItemInnerSpacingX", style.ItemInnerSpacing.x);
    YAML::KeyValue(node, "ItemInnerSpacingY", style.ItemInnerSpacing.y);
    YAML::KeyValue(node, "ItemSpacingX", style.ItemSpacing.x);
    YAML::KeyValue(node, "ItemSpacingY", style.ItemSpacing.y);
    YAML::KeyValue(node, "LogSliderDeadzone", style.LogSliderDeadzone);
    YAML::KeyValue(node, "MouseCursorScale", style.MouseCursorScale);
    YAML::KeyValue(node, "PopupBorderSize", style.PopupBorderSize);
    YAML::KeyValue(node, "PopupRounding", style.PopupRounding);
    YAML::KeyValue(node, "ScrollbarRounding", style.ScrollbarRounding);
    YAML::KeyValue(node, "ScrollbarSize", style.ScrollbarSize);
    YAML::KeyValue(node, "SelectableTextAlignX", style.SelectableTextAlign.x);
    YAML::KeyValue(node, "SelectableTextAlignY", style.SelectableTextAlign.y);
    YAML::KeyValue(node, "TabBorderSize", style.TabBorderSize);
    YAML::KeyValue(node, "TabMinWidthForCloseButton", style.TabMinWidthForCloseButton);
    YAML::KeyValue(node, "TabRounding", style.TabRounding);
    YAML::KeyValue(node, "TouchExtraPaddingX", style.TouchExtraPadding.x);
    YAML::KeyValue(node, "TouchExtraPaddingY", style.TouchExtraPadding.y);
    YAML::KeyValue(node, "WindowBorderSize", style.WindowBorderSize);
    YAML::KeyValue(node, "WindowMenuButtonPosition", (int)style.WindowMenuButtonPosition);
    YAML::KeyValue(node, "WindowMinSizeX", style.WindowMinSize.x);
    YAML::KeyValue(node, "WindowMinSizeY", style.WindowMinSize.y);
    YAML::KeyValue(node, "WindowPaddingX", style.WindowPadding.x);
    YAML::KeyValue(node, "WindowPaddingY", style.WindowPadding.y);
    YAML::KeyValue(node, "WindowRounding", style.WindowRounding);
    YAML::KeyValue(node, "WindowTitleAlignX", style.WindowTitleAlign.x);
    YAML::KeyValue(node, "WindowTitleAlignY", style.WindowTitleAlign.y);
    node << YAML::EndMap << YAML::EndMap;
}

void EStyle::Set(JWindow& window)
{
    ZoneScopedN("EStyle::Set");
    ImGuiStyle& currStyle = ImGui::GetStyle();
    window.currFont = font;
    window.currFontSize = fontSize;
    currStyle.Alpha = style.Alpha;
    currStyle.AntiAliasedFill = style.AntiAliasedFill;
    currStyle.AntiAliasedLines = style.AntiAliasedLines;
    currStyle.AntiAliasedLinesUseTex = style.AntiAliasedLinesUseTex;
    currStyle.ButtonTextAlign = style.ButtonTextAlign;
    currStyle.CellPadding = style.CellPadding;
    currStyle.ChildBorderSize = style.ChildBorderSize;
    currStyle.ChildRounding = style.ChildRounding;
    currStyle.CircleTessellationMaxError = style.CircleTessellationMaxError;
    currStyle.ColorButtonPosition = style.ColorButtonPosition;
    for (int i = 0; i < 53; i++)
    {
        currStyle.Colors[i] = style.Colors[i];
    }
    currStyle.ColumnsMinSpacing = style.ColumnsMinSpacing;
    currStyle.CurveTessellationTol = style.CurveTessellationTol;
    currStyle.DisplaySafeAreaPadding = style.DisplaySafeAreaPadding;
    currStyle.DisplayWindowPadding = style.DisplayWindowPadding;
    currStyle.FrameBorderSize = style.FrameBorderSize;
    currStyle.FramePadding = style.FramePadding;
    currStyle.FrameRounding = style.FrameRounding;
    currStyle.GrabMinSize = style.GrabMinSize;
    currStyle.GrabRounding = style.GrabRounding;
    currStyle.IndentSpacing = style.IndentSpacing;
    currStyle.ItemInnerSpacing = style.ItemInnerSpacing;
    currStyle.ItemSpacing = style.ItemSpacing;
    currStyle.LogSliderDeadzone = style.LogSliderDeadzone;
    currStyle.MouseCursorScale = style.MouseCursorScale;
    currStyle.PopupBorderSize = style.PopupBorderSize;
    currStyle.PopupRounding = style.PopupRounding;
    currStyle.ScrollbarRounding = style.ScrollbarRounding;
    currStyle.ScrollbarSize = style.ScrollbarSize;
    currStyle.SelectableTextAlign = style.SelectableTextAlign;
    currStyle.TabBorderSize = style.TabBorderSize;
    currStyle.TabMinWidthForCloseButton = style.TabMinWidthForCloseButton;
    currStyle.TabRounding = style.TabRounding;
    currStyle.TouchExtraPadding = style.TouchExtraPadding;
    currStyle.WindowBorderSize = style.WindowBorderSize;
    currStyle.WindowMenuButtonPosition = style.WindowMenuButtonPosition;
    currStyle.WindowMinSize = style.WindowMinSize;
    currStyle.WindowPadding = style.WindowPadding;
    currStyle.WindowRounding = style.WindowRounding;
    currStyle.WindowTitleAlign = style.WindowTitleAlign;
}