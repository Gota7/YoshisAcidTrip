#include <editor/styleEditor.hpp>

#include <editor/settings.hpp>
#include <editor/utils.hpp>
#include <jsystem/window.hpp>
#include <algorithm>
#include <imgui.h>

const JResPath THEME_FOLDER = JResPath("thm");

EStyleEditor::EStyleEditor(JWindow& window, ESettings& settings) : window(window), settings(settings)
{
    ZoneScopedN("EStyleEditor::EStyleEditor");
    Scan();
}

void EStyleEditor::Scan()
{
    ZoneScopedN("EStyleEditor::Scan");

    // Alphabetize themes and get current theme index.
    themes.clear();
    currTheme = 0;
    for (auto& file : std::filesystem::directory_iterator(std::filesystem::path(THEME_FOLDER.fullPath)))
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
        if (theme == settings.currTheme)
        {
            currTheme = ind;
            break;
        }
        ind++;
    }
    needsScan = false;

}

JResPath EStyleEditor::ThemePath(const std::string& name)
{
    ZoneScopedN("EStyleEditor::ThemePath");
    return JResPath(JFileSystem::PathSeparators(THEME_FOLDER.fullPath + "/" + name + ".ini"), true);
}

bool EStyleEditor::GetTheme(void* data, int idx, const char** outText)
{
    ZoneScopedN("EStyleEditor::GetTheme");
    EStyleEditor* ed = (EStyleEditor*)data;
    if ((std::size_t)idx >= ed->themes.size()) return false;
    else *outText = ed->themes[idx].c_str();
    return true;
}

bool EStyleEditor::GetFont(void* data, int idx, const char** outText)
{
    ZoneScopedN("EStyleEditor::GetFont");
    JWindow* win = (JWindow*)data;
    std::string& ret = win->GetFontByInd(idx);
    if (ret == "") return false;
    *outText = ret.c_str();
    return true;
}

void EStyleEditor::DrawUI()
{
    ZoneScopedN("EStyleEditor::DrawUI");
    if (needsScan) Scan();
    if (ImGui::Begin("Style Editor", &open))
    {

        // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to
        // (without a reference style pointer, we will use one compared locally as a reference)
        ImGuiStyle& style = ImGui::GetStyle();
        static ImGuiStyle ref_saved_style;

        // Theme menu. It will add and remove files as needed.
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);
        if (ImGui::Combo("Current Style", &currTheme, GetTheme, this, (int)themes.size()))
        {
            settings.currStyle = JPtrMake(EStyle, ThemePath(themes[currTheme]));
            settings.currStyle->Set(window);
            settings.currTheme = themes[currTheme];
            settings.Save();
        }
        std::string newName = themes[currTheme];
        if (EUtils::StringEdit("Style Name", newName) && newName != "")
        {
            std::filesystem::rename(ThemePath(themes[currTheme]).fullPath, ThemePath(newName).fullPath);
            settings.currTheme = newName;
            settings.Save();
            needsScan = true;
        }
        if (themes[currTheme] != "New Theme" && ImGui::Button("New Theme"))
        {
            settings.currTheme = "New Theme";
            needsScan = true;
            settings.currStyle->Save(ThemePath(settings.currTheme));
            settings.Save();
        }
        if (themes.size() > 1)
        {
            if (themes[currTheme] != "New Theme") { ImGui::SameLine(); }
            if (ImGui::Button("Delete Theme"))
            {
                std::filesystem::remove(ThemePath(themes[currTheme]).fullPath);
                settings.currStyle = JPtrMake(EStyle, ThemePath(themes[0]));
                settings.currStyle->Set(window);
                settings.currTheme = themes[0];
                settings.Save();
                needsScan = true;
            }
        }

        // Copied from the ImGui demo and slightly modified for the editor.
        if (ImGui::Button("Save Style"))
        {
            settings.currStyle->style = ref_saved_style = style;
            settings.currStyle->Save(ThemePath(settings.currTheme));
        }
        ImGui::SameLine();
        if (ImGui::Button("Revert Style"))
            style = settings.currStyle->style;

        ImGui::Separator();
        int currFontInd = window.GetIndByFont(settings.currStyle->font);
        if (ImGui::Combo("Font", &currFontInd, GetFont, &window, window.GetFontCount()))
        {
            window.currFont = settings.currStyle->font = window.GetFontByInd(currFontInd);
        }
        if (settings.currStyle->font != "Default" && ImGui::DragFloat("Font Size", &settings.currStyle->fontSize, 0.1f, 1.0f, 200.0f))
        {
            window.currFontSize = settings.currStyle->fontSize;
        }
        ImGui::SameLine();
        if (ImGui::Button("Scan Fonts")) window.needsFontScanned = true;

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Sizes"))
            {
                ImGui::Text("Main");
                ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
                ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
                ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
                ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
                ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
                ImGui::Text("Borders");
                ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
                ImGui::Text("Rounding");
                ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
                ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
                ImGui::Text("Alignment");
                ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
                int window_menu_button_position = style.WindowMenuButtonPosition + 1;
                if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                    style.WindowMenuButtonPosition = window_menu_button_position - 1;
                ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
                ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine(); EUtils::Tooltip("Alignment applies when a button is larger than its text content.");
                ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
                ImGui::SameLine(); EUtils::Tooltip("Alignment applies when a selectable is larger than its text content.");
                ImGui::Text("Safe Area Padding");
                ImGui::SameLine(); EUtils::Tooltip("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
                ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Colors"))
            {

                static ImGuiTextFilter filter;
                filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

                static ImGuiColorEditFlags alpha_flags = 0;
                if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None))             { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
                if (ImGui::RadioButton("Alpha",  alpha_flags == ImGuiColorEditFlags_AlphaPreview))     { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
                if (ImGui::RadioButton("Both",   alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } ImGui::SameLine();
                EUtils::Tooltip(
                    "In the color list:\n"
                    "Left-click on color square to open color picker,\n"
                    "Right-click to open edit options menu.");

                ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
                ImGui::PushItemWidth(-160);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const char* name = ImGui::GetStyleColorName(i);
                    if (!filter.PassFilter(name))
                        continue;
                    ImGui::PushID(i);
                    ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                    if (memcmp(&style.Colors[i], &settings.currStyle->style.Colors[i], sizeof(ImVec4)) != 0)
                    {
                        // Tips: in a real user application, you may want to merge and use an icon font into the main font,
                        // so instead of "Save"/"Revert" you'd use icons!
                        // Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
                        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) { settings.currStyle->style.Colors[i] = style.Colors[i]; }
                        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) { style.Colors[i] = settings.currStyle->style.Colors[i]; }
                    }
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                    ImGui::TextUnformatted(name);
                    ImGui::PopID();
                }
                ImGui::PopItemWidth();
                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Rendering"))
            {
                ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
                ImGui::SameLine();
                EUtils::Tooltip("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");

                ImGui::Checkbox("Anti-aliased lines use texture", &style.AntiAliasedLinesUseTex);
                ImGui::SameLine();
                EUtils::Tooltip("Faster lines using texture data. Require backend to render with bilinear filtering (not point/nearest filtering).");

                ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);

                ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();

}