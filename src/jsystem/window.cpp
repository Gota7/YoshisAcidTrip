#include <jsystem/window.hpp>

#include <jsystem/frameBuffer.hpp>
#include <texture/loader.hpp>
#include <imgui/imgui_impl_glfw.hpp>
#include <imgui/imgui_impl_opengl3.hpp>
#include <iostream>

// For resize callback.
JWindow* MainWindow;

const JResPath FONT_FOLDER = JResPath("fnt");

#if defined(__DEBUG__) && !defined(__APPLE__)
void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes
    //if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return; // Ignore notification.
    DBG_PRINT("OPENGL@JUNO (" << id << "): " <<  message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
#endif

void JWindowResize(GLFWwindow* window, int width, int height)
{
    MainWindow->SetSize(width, height);
}

JWindow::JWindow(const std::string& title)
{

    // Initialize main context.
    MainWindow = this;
    glfwInit();
#if defined(DEBUG) && !defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // We are really using version 3.1 but GLFW doesn't like that?
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(DEBUG) && !defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create main window.
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), NULL, NULL);
    if (!window)
    {
        const char* desc;
        int id = glfwGetError(&desc);
        printf("GLFW being big stupid :< %d: %s\n", id, desc);
        abort();
    }
    glfwSetWindowTitle(window, title.c_str());
    width = SCR_WIDTH;
    height = SCR_HEIGHT;
    if (window == NULL)
    {
        DBG_PRINT("Failed to create GLFW window. Error " << glfwGetError(NULL) << ".");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, JWindowResize);
    // glfwSwapInterval(0);

    // Load function pointers.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        DBG_PRINT("Failed to initialize GLAD");
        return;
    }

    // OpenGL debug context if needed.
#if defined(__DEBUG__) && !defined(__APPLE__)
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Makes sure errors are displayed synchronously.
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif
#ifdef __DEBUG__
    // TracyGpuContext;
#endif

    // Load icon.
    JPtr<TFormat> iconData = TLoader::LoadTexture(JResPath("tex/icon.png"));
    if (iconData)
    {
        int tmpChannels;
        iconData->Properties(icon.width, icon.height, tmpChannels);
        icon.pixels = (unsigned char*)iconData->DataPtr();
        glfwSetWindowIcon(window, 1, &icon);
    }
    else
    {
        DBG_PRINT("WARNING: Can not load tex/icon.png!");
    }

    // GL stuff.
    JFrameBuffer::CreateMainBuffer(width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // ImGui init.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = false;
    ImGui::StyleColorsDark();
    // ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 150";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

}

void JWindow::Main(const JWindowCallback& mainCallback, const JWindowCallback& renderCallback, const JWindowCallback& drawUICallback)
{

    // Main loop, pretty straitforward.
    while (!glfwWindowShouldClose(window))
    {
        if (mainCallback) mainCallback();
        JFrameBuffer::StartRender();
        if (renderCallback && (fps != 1 || needsRender)) renderCallback();
        JFrameBuffer::EndRender();
        UpdateFrame();
#ifdef __DEBUG__
        // TracyGpuCollect;
#endif
        {
            ZoneScopedN("GLFW Poll Events");
            glfwPollEvents();
        }

        // ImGui.
        if (needsFontScanned) DoScanFonts();
        if (oldFont != currFont) ImGui::GetIO().FontDefault = GetFont(currFont);
        if ((oldFontSize != currFontSize || oldFont != currFont) && currFont != "Default")
        {
            std::string path = JResPath(
                JFileSystem::PathSeparators(FONT_FOLDER.fullPath + "/" + currFont + ".ttf"),
                true
            ).fullPath;
            DBG_PRINT("FONT@LSD: Loading \"" << path << "\".");
            fonts[currFont] = ImGui::GetIO().Fonts->AddFontFromFileTTF(
                path.c_str(),
                currFontSize
            );
            DoScanFonts();
        }
        oldFont = currFont;
        oldFontSize = currFontSize;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (drawUICallback) drawUICallback();
        // ImGui::ShowDemoWindow();

#ifdef __DEBUG__
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
#endif
        if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS || (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS))
        {
            if (!fullscreenKeyDown)
            {
                const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                if (!fullscreen)
                {
                    glfwGetWindowPos(window, &posLast.x, &posLast.y);
                    sizeLast = glm::ivec2(width, height);
                }
                glfwSetWindowMonitor(
                    window,
                    fullscreen ? NULL : glfwGetPrimaryMonitor(),
                    fullscreen ? posLast.x : 0,
                    fullscreen ? posLast.y : 0,
                    fullscreen ? sizeLast.x : mode->width,
                    fullscreen ? sizeLast.y : mode->height,
                    GLFW_DONT_CARE
                );
                fullscreen = !fullscreen;
                fullscreenKeyDown = true;
            }
        }
        else fullscreenKeyDown = false;

         // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        FrameMark;

    }

}

void JWindow::SetSize(int width, int height)
{
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
    JFrameBuffer::MainBuffer().Resize(width, height);
}

void JWindow::GetSize(int& width, int& height) const
{
    width = this->width;
    height = this->height;
}

void JWindow::UpdateFrame()
{
    lastFrame = currentFrame;
    currentFrame = glfwGetTime();
    deltaTime = glm::min(currentFrame - lastFrame, DELTA_TIME_MAX);
    frameCounter++;
    if (currentFrame - lastSecond >= 1)
    {
        fps = frameCounter;
        frameCounter = 0;
        lastSecond = currentFrame;
    }
}

void JWindow::DoScanFonts()
{
    static bool scannedOnce = false;
    auto& io = ImGui::GetIO();
    if (!scannedOnce)
    {
        io.Fonts->ClearFonts();
        fonts.clear();
        fontKeys.clear();
        revFonts.clear();
        fontKeys.push_back("Default");
        fonts["Default"] = io.Fonts->AddFontDefault();
        revFonts["Default"] = 0;
    }
    for (auto& file : std::filesystem::directory_iterator(std::filesystem::path(FONT_FOLDER.fullPath)))
    {
        if (!file.is_directory() && file.path().extension().string() == ".ttf")
        {
            std::string fontName = file.path().filename().replace_extension("").string();
            if (fonts.find(fontName) == fonts.end())
            {
                revFonts[fontName] = fontKeys.size();
                fontKeys.push_back(fontName);
                DBG_PRINT("FONT@LSD: Loading \"" << file.path().string() << "\".");
                fonts[fontName] = io.Fonts->AddFontFromFileTTF(file.path().string().c_str(), currFontSize);
            }
        }
    }
    io.FontDefault = GetFont(currFont);
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    needsFontScanned = false;
    scannedOnce = true;
}

float JWindow::DeltaTime() const
{
    return (float)deltaTime;
}

void JWindow::SetFPS(unsigned int fps)
{
    targetFps = fps;
    assert(false); // NOT IMPLEMENTED!!!
    //glfwSwapInterval()
}

unsigned int JWindow::GetFPS() const
{
    return targetFps;
}

float JWindow::GetActualFPS() const
{
    return fps;
}

ImFont* JWindow::GetFont(const std::string& name)
{
    auto found = fonts.find(name);
    if (found == fonts.end()) return fonts["Default"];
    else return found->second;
}

std::string& JWindow::GetFontByInd(int ind)
{
    static std::string EMPTY = "";
    if (ind < 0 || ind >= (int)fontKeys.size()) return EMPTY;
    else return fontKeys[ind];
}

int JWindow::GetIndByFont(const std::string& name)
{
    auto found = revFonts.find(name);
    if (found == revFonts.end()) return 0;
    else return found->second;
}

int JWindow::GetFontCount() const
{
    return (int)fontKeys.size();
}

JWindow::~JWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // JFrameBuffer::CleanupMainBuffer();
    glfwDestroyWindow(window);
    glfwTerminate();
}