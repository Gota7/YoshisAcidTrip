#include <editor/editor.hpp>

#include <bin/formats/sarc.hpp>
#include <bin/formats/yaz0.hpp>
#include <bin/streams/file.hpp>
#include <bin/streams/memory.hpp>

EEditor::EEditor(JWindow& window) :
window(window),
shdTest(JResPath("shd/test.vert"), JResPath("shd/test.frag"), {}, 0),
renderBatch(1, JRenderBatchMode::Quads),
tex(JResPath("tex/icon.png"))
{
    ZoneScopedN("EEditor::EEditor");

    // Shaders.
    uniforms.Connect(shdTest);
    JVertexUV::SetShaderAttributes(shdTest);
    shdTest.SetInt("tex", 0);

    // Test.
    BStreamFile szs(JResPath("test/Pa1_1-1_1.szs"), BStreamMode::Read);
    BStreamMemory sarc(BStreamMode::ReadWrite);
    assert(Yaz0::Decompress(szs, sarc));
    sarc.Seek(0);
    SARC sarcFile(sarc);
    for (auto& filename : sarcFile.GetFilenames())
    {
        DBG_PRINT(filename);
    }
    BStreamFile bfres(JResPath("test/output.bfres"), BStreamMode::Write);
    auto& bfresMem = sarcFile.Open("output.bfres", BStreamMode::Read);
    bfres.Copy(bfresMem, bfresMem.Size());

}

void EEditor::Update()
{
    ZoneScopedN("EEditor::Update");
}

void EEditor::Render()
{
    ZoneScopedN("EEditor::Render");
    shdTest.Bind();
    tex.Bind(0);
    renderBatch.Begin();
    renderBatch.AddVertex({ glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) });
    renderBatch.AddVertex({ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) });
    renderBatch.AddVertex({ glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) });
    renderBatch.AddVertex({ glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });
    renderBatch.RenderAndFlush();
}