#include "editor.hpp"

#include "../bin/formats/byml.hpp"
#include "../bin/formats/sarc.hpp"
#include "../bin/formats/yaz0.hpp"
#include "../bin/streams/file.hpp"
#include "../bin/streams/memory.hpp"
#include <imgui.h>

EEditor::EEditor(JWindow& window) :
window(window),
settings(window),
styleEditor(window, settings),
shdTest(JResPath("shd/test.vert"), JResPath("shd/test.frag"), {}, 0),
renderBatch(1, JRenderBatchMode::Quads),
tex(JResPath("tex/icon.png"))
{
    ZoneScopedN("EEditor::EEditor");

    // Test.
    // BStreamFile sarcS(JResPath("tests/Pa1_1-1_1.sarc"), BStreamMode::Read);
    // SARC sarc(sarcS);
    // BStreamFile layoutS(JResPath("tests/Pa1_1-1_1.bin"), BStreamMode::Write);
    // BStreamFile objectsS(JResPath("tests/Pa1_1-1_1_hd.bin"), BStreamMode::Write);
    // BStreamFile colS(JResPath("tests/d_bgchk_Pa1_1-1_1.bin"), BStreamMode::Write);
    // auto& layout = sarc.Open("BG_unt/Pa1_1-1_1.bin", BStreamMode::Read);
    // auto& objects = sarc.Open("BG_unt/Pa1_1-1_1_hd.bin", BStreamMode::Read);
    // auto& col = sarc.Open("BG_chk/d_bgchk_Pa1_1-1_1.bin", BStreamMode::Read);
    // layoutS.Copy(layout, layout.Size());
    // objectsS.Copy(objects, objects.Size());
    // colS.Copy(col, col.Size());

    // // Shaders.
    // uniforms.Connect(shdTest);
    // JVertexUV::SetShaderAttributes(shdTest);
    // shdTest.SetInt("tex", 0);

    // Test.
    BStreamFile src(JResPath("tests/ActorInfo.Product.100.rstbl.byml"), BStreamMode::Read);
    BYML byml(src);
    byml.WriteYAML(JResPath("tests/ActorInfo.Product.100.rstbl.yaml"));

}

void EEditor::Update()
{
    ZoneScopedN("EEditor::Update");
}

void EEditor::DrawMainBar()
{
    ZoneScopedN("EEditor::DrawMainBar");
    bool openAboutPopup = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit", level != nullptr))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View", level != nullptr))
        {
            ImGui::EndMenu();
        }
        if (ImGui::Button("About"))
        {
            openAboutPopup = true;
        }
        ImGui::EndMainMenuBar();
    }
    if (openAboutPopup) ImGui::OpenPopup("About");
}

void EEditor::DrawAboutPopup()
{
    ZoneScopedN("EEditor::DrawAboutPopup");
    if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextColored(
            ImVec4(.4, 1, 0, 1),
            (
                "Yoshi's Acid Trip - Version " +
                std::to_string((unsigned int)settings.VERSION.y) +
                "." + std::to_string((unsigned int)settings.VERSION.z) +
                "." + std::to_string((unsigned int)settings.VERSION.w)
            ).c_str()
        );
        ImGui::TextColored(ImVec4(1, .1, .5, 1), "\tAn editor for Super Mario Bros. Wonder.");
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1, 0, 1, 1), "Gota7 - Everything.");
        ImGui::Separator();
        if (ImGui::Button("Ok"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void EEditor::DrawPopups()
{
    ZoneScopedN("EEditor::DrawPopups");
    DrawAboutPopup();
}

void EEditor::DrawUI()
{
    ZoneScopedN("EEditor::DrawUI");
    // ImGui::ShowDemoWindow();
    DrawMainBar();
    DrawPopups();
    styleEditor.DrawUI();
}

void EEditor::Render()
{
    ZoneScopedN("EEditor::Render");
    // shdTest.Bind();
    // tex.Bind(0);
    // renderBatch.Begin();
    // renderBatch.AddVertex({ glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) });
    // renderBatch.AddVertex({ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) });
    // renderBatch.AddVertex({ glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) });
    // renderBatch.AddVertex({ glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });
    // renderBatch.RenderAndFlush();
}