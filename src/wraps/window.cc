#include "window.hh"
#include <iostream>

namespace wr {
    Window::Window(unsigned width, unsigned height, const std::string& title,
                   const Context& context, bool vsync) {
        // Print useful information for finding out bugs
        // later on. This is also done with GLEW and GL.
        int glfw_major, glfw_minor, glfw_revis;
        glfwGetVersion(&glfw_major, &glfw_minor, &glfw_revis);
        std::cout << "GLFW Version " << glfw_major << '.'
                  << glfw_minor << '.' << glfw_revis << std::endl;

        glfwSetErrorCallback(error); // Throws exceptions!
        glfwInit(); // Will be handled by callbacks.

        // Construct context depending on input.
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context.minor);
        if (context.frwd) glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        if (context.core) glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!handle) glfwTerminate(); // If construction fails.. We are so screwed.
        glfwMakeContextCurrent(handle); // Still needs to load extensions, context is online.

        std::cout << "GLEW Version "
                  << glewGetString(GLEW_VERSION)
                  << std::endl;

        // Fetch all driver functions.
        glewExperimental = GL_TRUE;
        GLenum glew_error {glewInit()};
        if (glew_error != GLEW_OK) {
            const char* message = (const char*)glewGetErrorString(glew_error);
            glfwDestroyWindow(handle); glfwTerminate(); // Still in constructor...
            error(5318008, message); // Why not... GLFW uses big numbers.
        }

        std::cout << "OpenGL Version " << glGetString(GL_VERSION) << " by "
                  << glGetString(GL_VENDOR) << " with GLSL Version "
                  << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "OpenGL Renderer " << glGetString(GL_RENDERER) << std::endl;

        if (vsync) glfwSwapInterval(1);
        glfwSetKeyCallback(handle, input);
    }

    Window::~Window() {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    void Window::input(GLFWwindow* handle, int key,
                       int scan, int action, int mods) {
    }
}
