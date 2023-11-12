#include <editor/editor.hpp>
#include <jsystem/fs.hpp>
#include <jsystem/window.hpp>

JPtr<EEditor> MainEditor;

void Update()
{
    MainEditor->Update();
}

void Render()
{
    MainEditor->Render();
}

void DrawUI()
{
    MainEditor->DrawUI();
}

int main()
{
    JWindow window(APP_TITLE);
    MainEditor = JPtrMake(EEditor, window);
    DBG_PRINT("BUILDTIME@LSD: " << JFileSystem::ReadAllLines(JResPath("BUILDTIME")));
    window.Main(Update, Render, DrawUI);
    MainEditor.reset();
    return 0;
}