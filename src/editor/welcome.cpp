#include "welcome.hpp"

#include "editor.hpp"
#include "utils.hpp"
#include <clip.h>
#include <imgui.h>

EWelcome::EWelcome(EEditor& editor) : editor(editor)
{
    ZoneScopedN("EWelcome::EWelcome");
    editor.settings.ReloadLangList();
    editor.settings.ReloadThemeList();
}

bool EWelcome::Popped()
{
    ZoneScopedN("EWelcome::Popped");
    return popped;
}

void EWelcome::Popup()
{
    ZoneScopedN("EWelcome::Popup");
    ImGui::OpenPopup(editor.text["Welcome!"].c_str());
    popped = true;
}

void EWelcome::DrawUI()
{
    ZoneScopedN("EWelcome::DrawUI");
    if (ImGui::BeginPopupModal(editor.text["Welcome!"].c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {

        // Intro.
        ImGui::BeginTabBar("WelcomeTabs", ImGuiTabBarFlags_Reorderable);
        if (ImGui::BeginTabItem(editor.text["Settings"].c_str()))
        {
            ImGui::TextColored(ImVec4(1, .1, .5, 1), editor.text["Welcome_Intro"].c_str());
            ImGui::TextColored(ImVec4(1, 0.6f, 0, 1), editor.text["Welcome_Intro_Settings"].c_str());
            if (editor.settings.DrawLangSelector(editor))
            {
                editor.statusText = editor.text["Welcome_To_Editor"];
            }
            editor.settings.DrawThemeSelector(editor);
            ImGui::EndTabItem();
        }

        // Mod settings.
        if (ImGui::BeginTabItem(editor.text["Mod"].c_str()))
        {
            ImGui::TextColored(ImVec4(1, 0.6f, 0, 1), editor.text["Welcome_Intro_Guide1"].c_str());
            ImGui::TextColored(ImVec4(1, 1, 0, 1), editor.text["Welcome_Intro_Guide2"].c_str());
            ImGui::TextColored(ImVec4(0, 1, 0, 1), editor.text["Welcome_Intro_Guide3"].c_str());
            ImGui::TextColored(ImVec4(1, 0.6f, 0, 1), editor.text["Welcome_Intro_Guide4"].c_str());
            bool hovered = EUtils::Tooltip(editor, editor.text["Welcome_Intro_Billiam"].c_str())
                        | EUtils::Tooltip(editor, editor.text["Welcome_Intro_Filipio"].c_str(), ERR_FLOWER);
            if (progress == 0 && hovered) progress = 1;
            else if (progress == 0) ImGui::TextColored(ImVec4(1, 0, 0, 1), editor.text["Welcome_Intro_Demand"].c_str());

            // ROMFS selection.
            if (progress > 0)
            {

                // How this works.
                ImGui::Separator();
                ImGui::TextColored(ImVec4(1, 0, 0, 1), editor.text["Romfs_Folder_Conv_Explain"].c_str());
                ImGui::TextColored(ImVec4(1, 1, 0, 1), editor.text["Mod_Folder_Use_Explain"].c_str());
                ImGui::TextColored(ImVec4(1, .1, .5, 1), editor.text["Mod_Folder_Conv_Explain"].c_str());
                ImGui::Separator();

                // Romfs.
                ImGui::Checkbox(editor.text["Create_Deserialized_Folder"].c_str(), &fromRomfs);
                EUtils::Tooltip(editor, editor.text["Create_Deserialized_Folder_Explain"]);
                bool isMore = !fromRomfs;
                if (fromRomfs)
                {
                    EUtils::StringEdit(editor.text["Romfs_Folder"], romfsPath);
                    ImGui::SameLine();
                    if (ImGui::Button("...##RomfsFolderSelect"))
                    {
                        ImGui::OpenPopup(editor.text["Romfs_Folder_Select"].c_str());
                    }
                    if (fb.showFileDialog(editor.text["Romfs_Folder_Select"], imgui_addons::ImGuiFileBrowser::DialogMode::SELECT))
                    {
                        romfsPath = fb.selected_path;
                    }
                    bool rightClicked = false;
                    EUtils::Tooltip(editor, editor.text["Romfs_Folder_Explain"], NORMAL_FLOWER, &rightClicked);
                    if (rightClicked)
                    {
                        clip::set_text("https://www.reddit.com/r/PokemonBDSPmodding/comments/r1bijk/guide_how_to_dump_romfs_using_yuzu/");
                        editor.statusText = editor.text["Copied_Reddit_Link"];
                    }
                    if (!std::filesystem::exists(romfsPath))
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Does_Not_Exist"], ERR_FLOWER);
                        isMore = false;
                    }
                    else if (std::filesystem::exists(romfsPath + "/LSD_VERSION.yaml") || std::filesystem::exists(romfsPath + "/LSD_CONVERSIONS.yaml"))
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Is_Deserialized_Or_Mod_Folder"], ERR_FLOWER);
                        isMore = false;
                    }
                    else if (!std::filesystem::exists(romfsPath + "/RSDB/StageInfo.Product.100.rstbl.byml.zs")) // Hardcoded file that should be there lol.
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Bad_Romfs"], ERR_FLOWER);
                        isMore = false;
                    }
                    else isMore = true;
                }

                // Deserialized path.
                if (isMore)
                {
                    EUtils::StringEdit(editor.text["Deserialized_Folder"], basePath);
                    ImGui::SameLine();
                    if (ImGui::Button("...##DeserializedFolderSelect"))
                    {
                        ImGui::OpenPopup(editor.text["Deserialized_Folder_Select"].c_str());
                    }
                    if (fb.showFileDialog(editor.text["Deserialized_Folder_Select"], imgui_addons::ImGuiFileBrowser::DialogMode::SELECT))
                    {
                        basePath = fb.selected_path;
                    }
                    EUtils::Tooltip(editor, editor.text["Deserialized_Folder_Explain"]);
                    if (!std::filesystem::exists(basePath))
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Does_Not_Exist"], ERR_FLOWER);
                        isMore = false;
                    }
                    else if (std::filesystem::exists(basePath + "/RSDB/StageInfo.Product.100.rstbl.byml.zs")) // Hardcoded file that should be there lol.
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Is_Romfs"], ERR_FLOWER);
                        isMore = false;
                    }
                    else if (!fromRomfs && !std::filesystem::exists(basePath + "/LSD_VERSION.yaml"))
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Is_Not_Deserialized_Folder"], ERR_FLOWER);
                        isMore = false;
                    }
                }

                // Mod path.
                if (isMore)
                {
                    EUtils::StringEdit(editor.text["Mod_Folder"], patchPath);
                    ImGui::SameLine();
                    if (ImGui::Button("...##ModFolderSelect"))
                    {
                        ImGui::OpenPopup(editor.text["Mod_Folder_Select"].c_str());
                    }
                    if (fb.showFileDialog(editor.text["Mod_Folder_Select"], imgui_addons::ImGuiFileBrowser::DialogMode::SELECT))
                    {
                        patchPath = fb.selected_path;
                    }
                    EUtils::Tooltip(editor, editor.text["Mod_Folder_Explain"]);
                    if (!std::filesystem::exists(patchPath))
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Does_Not_Exist"], ERR_FLOWER);
                        isMore = false;
                    }
                    else if (std::filesystem::exists(patchPath + "/RSDB/StageInfo.Product.100.rstbl.byml.zs")) // Hardcoded file that should be there lol.
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Is_Romfs"], ERR_FLOWER);
                        isMore = false;
                    }
                }

                // Build path.
                if (isMore)
                {
                    EUtils::StringEdit(editor.text["Build_Folder"], buildPath);
                    ImGui::SameLine();
                    if (ImGui::Button("...##BuildFolderSelect"))
                    {
                        ImGui::OpenPopup(editor.text["Build_Folder_Select"].c_str());
                    }
                    if (fb.showFileDialog(editor.text["Build_Folder_Select"], imgui_addons::ImGuiFileBrowser::DialogMode::SELECT))
                    {
                        buildPath = fb.selected_path;
                    }
                    EUtils::Tooltip(editor, editor.text["Build_Folder_Explain"]);
                    if (!std::filesystem::exists(buildPath))
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Does_Not_Exist"], ERR_FLOWER);
                        isMore = false;
                    }
                    else if (std::filesystem::exists(buildPath + "/RSDB/StageInfo.Product.100.rstbl.byml.zs")) // Hardcoded file that should be there lol.
                    {
                        EUtils::Tooltip(editor, editor.text["Path_Is_Romfs"], ERR_FLOWER);
                        isMore = false;
                    }
                }

                // Mod name.
                if (isMore)
                {
                    EUtils::StringEdit(editor.text["Mod_Name"], modName);
                    if (modName == "")
                    {
                        EUtils::Tooltip(editor, editor.text["Mod_Name_Needed"], ERR_FLOWER);
                        isMore = false;
                    }
                    else EUtils::Tooltip(editor, editor.text["Mod_Name_Appreciated"]);
                }

                // No more, some kind of error.
                if (!isMore)
                {
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), editor.text["Wondering_How_To_Continue"].c_str());
                    EUtils::Tooltip(editor, editor.text["Wondering_How_To_Continue_Explain"], ERR_FLOWER);
                    progress = 1;
                }

                // All valid, let user continue.
                else progress = 2;

            }
            ImGui::EndTabItem();

        }

        // Finish popup.
        ImGui::EndTabBar();
        ImGui::EndPopup();

    }
}