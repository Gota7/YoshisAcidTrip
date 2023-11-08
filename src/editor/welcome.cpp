#include "welcome.hpp"

#include "editor.hpp"
#include "utils.hpp"
#include <clip.h>
#include <imgui.h>

EWelcome::EWelcome(EEditor& editor) : editor(editor)
{
    ZoneScopedN("EWelcome::EWelcome");
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
        ImGui::TextColored(ImVec4(1, .1, .5, 1), editor.text["Welcome_Intro"].c_str());
        ImGui::Separator();
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
            ImGui::Checkbox("Create Base Folder From Romfs", &fromRomfs);
            EUtils::Tooltip(editor, "Check this if you need to create a base folder for your mod using the Romfs of Mario Wonder.\nYou should keep this checked if this is your first time running the editor or lost your romfs base folder.");
            bool isMore = !fromRomfs;
            if (fromRomfs)
            {
                EUtils::StringEdit(editor.text["Romfs Folder"].c_str(), romfsPath);
                bool rightClicked = false;
                EUtils::Tooltip(editor, "Path to a completely unmodified romfs folder of Mario Wonder.\nRight click me to copy a link on how to dump the romfs for the game to your clipboard!", NORMAL_FLOWER, &rightClicked);
                if (rightClicked)
                {
                    clip::set_text("https://www.reddit.com/r/PokemonBDSPmodding/comments/r1bijk/guide_how_to_dump_romfs_using_yuzu/");
                    editor.statusText = "Copied reddit link to clipboard.";
                }
                if (!std::filesystem::exists(romfsPath))
                {
                    EUtils::Tooltip(editor, "The path selected does not exist!\nTry again!", ERR_FLOWER);
                    isMore = false;
                }
                else if (std::filesystem::exists(romfsPath + "/LSD_VERSION.yaml") || std::filesystem::exists(romfsPath + "/LSD_CONVERSIONS.yaml"))
                {
                    EUtils::Tooltip(editor, "The path selected is either a deserialized folder or a mod folder!\nTry again!", ERR_FLOWER);
                    isMore = false;
                }
                else if (!std::filesystem::exists(romfsPath + "/RSDB/StageInfo.Product.100.rstbl.byml.zs")) // Hardcoded file that should be there lol.
                {
                    EUtils::Tooltip(editor, "The path selected is not a valid romfs dump!\nMake sure it is not inside another folder in the folder you selected\nTry again!", ERR_FLOWER);
                    isMore = false;
                }
                else isMore = true;
            }

            // Deserialized path.
            if (isMore)
            {
                EUtils::StringEdit(editor.text["Deserialized_Folder"].c_str(), basePath);
                EUtils::Tooltip(editor, "If \"Create Base Folder From Romfs\" is checked, this will be where to deserialize the romfs to.\nDeserializing the romfs lets you and the editor view and edit the files more easily.\nFor example, BYML is converted to YAML, SARCs are extracted, ZS files decompressed, images converted, etc.\nIf \"Create Base Folder From Romfs\" is not checked, this should be the path to an already deserialized romfs.");
                // TODO: FIGURE OUT HOW TO CHECK THIS!!!
            }

            // Mod path.
            if (isMore)
            {
                EUtils::StringEdit(editor.text["Mod_Folder"].c_str(), patchPath);
                EUtils::Tooltip(editor, "This is where your mod's files will be stored.\nNote that the files here are deserialized to be as editable as possible like in the deserialized folder.\nThink of the files here as a replacement for files in the deserialized romfs folder.\nIf a file is not found here, it will be loaded from the deserialized folder.\nNote that no files will or ever should be modified in the deserialized folder!");
                // TODO: FIGURE OUT HOW TO CHECK THIS!!!
            }

            // Build path.
            if (isMore)
            {
                EUtils::StringEdit(editor.text["Build_Folder"].c_str(), buildPath);
                EUtils::Tooltip(editor, "You can not play a mod folder without converting/serializing it back first!\nThis is the folder where your mod will be converted to something playable or sharable for people to play.\nThis should probably be a mod romfs path in your emulator for easier playback.");
                // TODO: FIGURE OUT HOW TO CHECK THIS!!!
            }

            // Mod name.
            if (isMore)
            {
                EUtils::StringEdit(editor.text["Mod_Name"].c_str(), modName);
                if (modName == "")
                {
                    EUtils::Tooltip(editor, "Let's give your mod a name!\nDon't worry, you can change it later!", ERR_FLOWER);
                    isMore = false;
                }
                else EUtils::Tooltip(editor, "I like it!\nIf you don't like it in the future, you can change it in the settings window!");
            }

            // No more, some kind of error.
            if (!isMore)
            {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Wondering how to continue?");
                EUtils::Tooltip(editor, "There is no button here because either you didn't select a path or the path chosen is invalid!\nMake sure to read what we say if you are confused and to check the lowest path on the list!\nI show up on errors, so hover over me above if you see me!", ERR_FLOWER);
                progress = 1;
            }

            // All valid, let user continue.
            else progress = 2;

        }

        // Finish popup.
        ImGui::EndPopup();

    }
}