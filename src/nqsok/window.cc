#include "window.hh"

nq::Window::Window(int width, int height, const std::string& title,
                   const nq::Context& context, bool fullscreen = false, bool vsync = true) {
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

    glewExperimental = GL_TRUE;
    GLenum glew_state {glewInit()};
    if (glew_state != GLEW_OK) {
        glfwTerminate(); // Should destroy all opened windows (hopefully).
        const char* cmessage {(const char*)glewGetErrorString(glew_state)};
        std::string message {"Context error (#"};
        message += std::to_string(glew_state);
        message += ") "; message += cmessage;
        throw Context_error{message};
    }

    // Might not be honoured according to the GLFW documentation.
    // This is dependent on the GPU vendor driver it seems...
    if (vsync) glfwSwapInterval(1);
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
