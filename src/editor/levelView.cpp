#include "levelView.hpp"

#include <imgui.h>

ELevelView::ELevelView() : view(WIDTH_DEFAULT, HEIGHT_DEFAULT)
{
    ZoneScopedN("ELevelView::ELevelView");
    view.bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
}

void ELevelView::DrawUI(const std::string& levelName)
{
    ZoneScopedN("ELevelView::DrawUI");
    shown = ImGui::Begin(("Level View - " + levelName).c_str());
    if (shown)
    {
        JTexture& tex = view.FrameBuffer().GetTex();
        ImVec2 bounds = ImGui::GetContentRegionAvail();
        view.width = bounds.x;
        view.height = bounds.y;
        ImGui::Image((ImTextureID)(uintptr_t)tex.GetID(), ImVec2(tex.Width(), tex.Height()));
    }
    ImGui::End();
}

void ELevelView::Render(YAML::Node& level)
{
    ZoneScopedN("ELevelView::Render");
    if (shown)
    {
        view.FrameBegin();
        view.DrawLine(glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
        view.FrameEnd();
    }
}