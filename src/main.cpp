#include <constants.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define APP_TITLE "Yoshi's Acid Trip"

int main()
{

    // GLFW stuff.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // We are really using version 3.1 but GLFW doesn't like that?
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create main window.
    GLFWwindow* window = glfwCreateWindow(1024, 576, APP_TITLE, NULL, NULL);
    glfwSetWindowTitle(window, APP_TITLE);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window. Error " << glfwGetError(NULL) << "." << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(0);

    // Load function pointers.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    // Main loop, pretty straitforward.
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
#ifdef __DEBUG__
        // TracyGpuCollect;
#endif
        {
            ZoneScopedN("GLFW Poll Events");
            glfwPollEvents();
        }
#ifdef __DEBUG__
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
#endif
        // if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS || (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS))
        // {
        //     if (!WindowFullscreenKeyDown)
        //     {
        //         const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        //         if (!WindowFullscreen)
        //         {
        //             glfwGetWindowPos(MainWindow.window, &WindowPosLast.x, &WindowPosLast.y);
        //             WindowSizeLast = glm::ivec2(MainWindowWidth, MainWindowHeight);
        //         }
        //         glfwSetWindowMonitor(
        //             MainWindow.window,
        //             WindowFullscreen ? NULL : glfwGetPrimaryMonitor(),
        //             WindowFullscreen ? WindowPosLast.x : 0,
        //             WindowFullscreen ? WindowPosLast.y : 0,
        //             WindowFullscreen ? WindowSizeLast.x : mode->width,
        //             WindowFullscreen ? WindowSizeLast.y : mode->height,
        //             GLFW_DONT_CARE
        //         );
        //         WindowFullscreen = !WindowFullscreen;
        //         WindowFullscreenKeyDown = true;
        //     }
        // }
        // else WindowFullscreenKeyDown = false;
        FrameMark;
    }

}