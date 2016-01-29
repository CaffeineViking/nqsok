#include "window.hh"
#include <iostream>

nq::Window::Window(int width, int height, const std::string& title,
                   const Context& context, bool fullscreen = false, bool vsync = true) {
    std::cout << "\nWindow (creating)..." << std::endl;
    report_glfw(); // Gives library information, for debugging.
    glfwSetErrorCallback(error); // Will throw exception...
    glfwInit(); // Handled by error callback.

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Why wouldn't you?!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context.major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context.minor_version);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, context.forward_compatible);
    if (context.core_profile) glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor {nullptr};
    if (fullscreen) monitor = glfwGetPrimaryMonitor();
    handle = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    // Errors thrown by window creation will be "handled" (heh) in the callback.

    // Note below, in order for the user to swap between contexts, a manual function is needed.
    glfwMakeContextCurrent(handle); // Meaning, last window created will have current context.

    report_glew();
    glewExperimental = GL_TRUE;
    GLenum glew_state {glewInit()};
    if (glew_state != GLEW_OK) {
        glfwTerminate(); // Should destroy all opened windows (hopefully).
        const char* cmessage {(const char*)glewGetErrorString(glew_state)};
        error(glew_state, cmessage);
    }

    report_opgl(); // Contains a lot of useful information for debugging.

    // Might not be honoured according to the GLFW documentation.
    // This is dependent on the GPU vendor driver it seems...
    if (vsync) glfwSwapInterval(1);
    std::cout << "...done (Window)" << std::endl;
}

// Note that glfwTerminate is not here. That needs to
// be done elsewere since it closes *all* windows, not
// only this one. Maybe create a library singleton?
nq::Window::~Window() { glfwDestroyWindow(handle); }

void nq::Window::error(int code, const char* cmessage) {
    std::string message {"Window error (#"};
    message += std::to_string(code);
    message += ") "; message += cmessage;
    throw Window_error{message};
}

void nq::Window::report_glfw() const {
    std::cout << "GLFW version "
              << glfwGetVersionString()
              << std::endl;
}

void nq::Window::report_glew() const {
    std::cout << "GLEW version "
              << glewGetString(GLEW_VERSION)
              << std::endl;
}

void nq::Window::report_opgl() const {
    std::cout << "OpenGL version "
              << glGetString(GL_VERSION)
              << "\nOpenGL vendor "
              << glGetString(GL_VENDOR)
              << "\nOpenGL renderer "
              << glGetString(GL_RENDERER)
              << "\nGLSL version "
              << glGetString(GL_SHADING_LANGUAGE_VERSION)
              << std::endl;
}
