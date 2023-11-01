#pragma once

#include "../jsystem/texture.hpp"
#include "../jsystem/window.hpp"
#include "editor.hpp"
#include <imgui.h>
#include <string>

// ImGui utils.
namespace EUtils
{

    // Draw a texture. Will automatically do texture's default size.
    inline void DrawTex(const JTexture& tex, const ImVec2& size = ImVec2(-1.0f, -1.0f), const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tintCol = ImVec4(1, 1, 1, 1), const ImVec4& borderCol = ImVec4(0, 0, 0, 0))
    {
        ImVec2 sizeNew = size;
        if (size.x < 0.0f) sizeNew.x = tex.Width();
        if (size.y < 0.0f) sizeNew.y = tex.Height();
        ImGui::Image((ImTextureID)(uintptr_t)tex.GetID(), sizeNew, uv0, uv1, tintCol, borderCol);
    }

    // Show a tooltip when hovered.
    inline bool Tooltip(EEditor& editor, const std::string& tooltip, const ImVec4& color = ImVec4(0.95f, 0.75f, 0.25f, 1))
    {
        static float texFrame = 0.0f;
        ImGui::SameLine();
        // ImGui::TextDisabled("(?)");
        float lineHeight = ImGui::GetTextLineHeight();
        float lineHeightSpacing = ImGui::GetTextLineHeightWithSpacing();
        float sizeExtra = (lineHeightSpacing - lineHeight) / 2.0f;
        ImVec2 size((lineHeightSpacing + sizeExtra) / 2.0f, lineHeightSpacing + sizeExtra);
        float bakCursor = ImGui::GetCursorPosX();
        float cursorY = ImGui::GetCursorPosY() - (lineHeightSpacing - lineHeight) / 2.0f - sizeExtra;
        ImGui::SetCursorPosY(cursorY);
        DrawTex(editor.TalkingFlowerFrame((int)texFrame), size, ImVec2(0, 0), ImVec2(1, 1), color);
        bool hovered = ImGui::IsItemHovered();
        ImGui::SameLine();
        ImGui::SetCursorPosY(cursorY);
        ImGui::SetCursorPosX(bakCursor + size.x);
        DrawTex(editor.TalkingFlowerFrame((int)texFrame), size, ImVec2(1, 0), ImVec2(0, 1), color);
        if (hovered || ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::SetTooltip("%s", tooltip.c_str());
            ImGui::EndTooltip();
            texFrame += editor.window.DeltaTime() * 25.0f;
            if (texFrame >= editor.NumTalkingFlowerFrames()) texFrame = 0;
            return true;
        }
        return false;
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