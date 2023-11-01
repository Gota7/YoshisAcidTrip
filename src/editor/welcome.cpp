#include "welcome.hpp"

#include "editor.hpp"
#include "utils.hpp"
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
    ImGui::OpenPopup("Welcome!");
    popped = true;
}

void EWelcome::DrawUI()
{
    ZoneScopedN("EWelcome::DrawUI");
    if (ImGui::BeginPopupModal("Welcome!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextColored(ImVec4(1, .1, .5, 1), "Welcome to Yoshi's Acid Trip!\nA lucid way to edit Super Mario Bros. Wonder for the Nintendo Switch (allegedly).");
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1, 0.6f, 0, 1), "Before we can start editing the game, we must walk you through setting up a mod.");
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Separating the original romfs from your mod folder allows you to better organize your files, save storage space, and edit data easier.");
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "In the process we will also convert your romfs to common formats like text and images to make things even easier to edit!");
        ImGui::TextColored(ImVec4(1, 0.6f, 0, 1), "You will also come across many [talking flowers] to help you if you get confused.");
        bool hovered = EUtils::Tooltip(editor, "Hi, you found me!\nMy name is Billiam!");
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Hover over them for more info. Do it now... or not, we can wait here all day.");
        if (progress == 0 && hovered) progress = 1;
        if (progress > 0)
        {
            ImGui::Separator();
            ImGui::Text("TODO!");
        }
        ImGui::EndPopup();
    }
}