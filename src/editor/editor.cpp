#include "editor.hpp"

#include "../bin/formats/byml.hpp"
#include "../bin/formats/sarc.hpp"
#include "../bin/formats/zstd.hpp"
#include "../bin/streams/file.hpp"
#include "../bin/streams/memory.hpp"
#include "../fs/convert.hpp"
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

    // Shaders.
    // uniforms.Connect(shdTest);
    // JVertexUV::SetShaderAttributes(shdTest);
    // shdTest.SetInt("tex", 0);

    // Test.
    // BStreamFile src(JResPath("tests/ActorInfo.Product.100.rstbl.byml"), BStreamMode::Read);
    // BYML byml(src);
    // byml.WriteYAML(JResPath("tests/ActorInfo.Product.100.rstbl.yaml"));

    // Compression.
    // BStreamFile src(JResPath("tests/Default.Nin_NX_NVN.genvb.zs"), BStreamMode::Read);
    // BStreamFile dst(JResPath("tests/Default.Nin_NX_NVN.genvb"), BStreamMode::Write);
    // ZSTD::Decompress(src, dst, src.Size());
    // BYML byml(src);
    // byml.WriteYAML(JResPath("tests/ActorInfo.Product.100.rstbl.yaml"));
    // auto romfs = *FConvert::MakeEditorRomfs(JResPath("romfs"), JResPath("tests/TestMod/Base"), JResPath("tests/TestMod/Patch"));
    // FRomfs romfs(JResPath("tests/TestMod/Base"), JResPath("tests/TestMod/Patch"));
    // FConvert::UpdateRomfs(romfs);

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