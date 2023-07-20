#include <editor/editor.hpp>

EEditor::EEditor(JWindow& window) :
window(window),
shdTest(JResPath("shd/test.vert"), JResPath("shd/test.frag"), {}, 0),
renderBatch(1, JRenderBatchMode::Triangles)
{
    ZoneScopedN("EEditor::EEditor");

    // Shaders.
    uniforms.Connect(shdTest);
    JVertexUV::SetShaderAttributes(shdTest);

}

void EEditor::Update()
{
    ZoneScopedN("EEditor::Update");
}

void EEditor::Render()
{
    ZoneScopedN("EEditor::Render");
    shdTest.Bind();
    renderBatch.Begin();
    renderBatch.AddVertex({ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) });
    renderBatch.AddVertex({ glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) });
    renderBatch.AddVertex({ glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });
    renderBatch.RenderAndFlush();
}