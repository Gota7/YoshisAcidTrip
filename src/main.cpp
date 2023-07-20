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

int main()
{
    JWindow window(APP_TITLE);
    MainEditor = JPtrMake(EEditor, window);
    DBG_PRINT("BUILDTIME@JUNO: " << JFileSystem::ReadAllLines(JResPath("BUILDTIME")));
    window.Main(Update, Render);
    MainEditor.reset();
    return 0;
}