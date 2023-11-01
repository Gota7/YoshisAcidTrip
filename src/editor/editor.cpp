#include "editor.hpp"

#include "../bin/formats/byml.hpp"
#include "../bin/formats/sarc.hpp"
#include "../bin/formats/zstd.hpp"
#include "../bin/streams/file.hpp"
#include "../bin/streams/memory.hpp"
#include "../fs/convert.hpp"
#include <format>
#include <imgui.h>

EEditor::EEditor(JWindow& window) :
window(window),
settings(window),
styleEditor(*this, settings),
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
    // BStreamFile src(JResPath("tests/TestMod/Base/Effect/TimelineEventEffectKoopaTransform.Nin_NX_NVN.esetb.byml"), BStreamMode::Read);
    // BStreamFile dst(JResPath("tests/Default.Nin_NX_NVN.genvb"), BStreamMode::Write);
    // ZSTD::Decompress(src, dst, src.Size());
    // BYML byml(src);
    // DBG_PRINT(byml.WriteYAML(JResPath("tests/TestMod/Base/Effect/TimelineEventEffectKoopaTransform.Nin_NX_NVN.esetb.yaml")));
    // auto romfs = *FConvert::MakeEditorRomfs(JResPath("romfs"), JResPath("tests/TestMod/Base"), JResPath("tests/TestMod/Patch"));
    // FRomfs romfs(JResPath("tests/TestMod/Base"), JResPath("tests/TestMod/Patch"));
    // FConvert::UpdateRomfs(romfs, true);

    // Talking flowers!
    unsigned int imageInd = 0;
    while (true)
    {
        JResPath path = JResPath("tex/flower/" + std::format("{:02}.png", imageInd++));
        if (JFileSystem::FileExists(path))
        {
            talkingFlowerFrames.emplace_back(JPtrMake(JTexture, path));
        }
        else break;
    }
    if (talkingFlowerFrames.size() == 0)
    {
        DBG_PRINT("BLABBER@LSD: Unable to load Billiam's files in \"res/tex/flower\". You can silence him but you can't silence the truth.");
        abort();
    }

    // Load settings.
    settings.Load(window);
    auto mod = settings.mods.find(settings.currMod);
    if (mod == settings.mods.end()) // TODO: TEST TO MAKE SURE MOD IS VALID!
    {
        welcomeWindow = JPtrMake(EWelcome, *this);
    }

}

std::size_t EEditor::NumTalkingFlowerFrames() const
{
    ZoneScopedN("EEditor::NumTalkingFlowerFrames");
    return talkingFlowerFrames.size();
}

JTexture& EEditor::TalkingFlowerFrame(std::size_t ind)
{
    ZoneScopedN("EEditor::TalkingFlowerFrame");
    if (ind >= talkingFlowerFrames.size()) ind = 0;
    return *talkingFlowerFrames[ind];
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
                std::to_string(settings.VERSION[VER_INDEX_MAJOR]) +
                "." + std::to_string(settings.VERSION[VER_INDEX_MAJOR]) +
                "." + std::to_string(settings.VERSION[VER_INDEX_REV])
            ).c_str()
        );
        ImGui::TextColored(ImVec4(1, .1, .5, 1), "\tAn editor for Super Mario Bros. Wonder.");
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1, 0, 1, 1), "Gota7 - Everything.");
        ImGui::TextColored(ImVec4(1, 0.6f, 0, 1), "Payanica - Ideas, resources, testing.");
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Ndymario - Mac support and testing.");
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
    if (welcomeWindow && !welcomeWindow->Popped()) welcomeWindow->Popup();
    if (welcomeWindow) welcomeWindow->DrawUI();
}

void EEditor::DrawUI()
{
    ZoneScopedN("EEditor::DrawUI");
    // ImGui::ShowDemoWindow();
    DrawMainBar();
    DrawPopups();
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