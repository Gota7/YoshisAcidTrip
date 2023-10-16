#pragma once

#include <imgui.h>
#include <string>

// ImGui utils.
namespace EUtils
{

    // Show a tooltip when hovered.
    inline void Tooltip(const std::string& tooltip)
    {
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::SetTooltip("%s", tooltip.c_str());
            ImGui::EndTooltip();
        }
    }

    // Edit a string. Has a limit of 128 chars.
    inline bool StringEdit(const std::string& label, std::string& toEdit, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
    {
        char buf[128];
        strcpy(buf, toEdit.data());
        bool ret = ImGui::InputText(label.c_str(), buf, 128, flags);
        if (ret) toEdit = std::string(buf);
        return ret;
    }

}