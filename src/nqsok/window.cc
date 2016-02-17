#include "window.hh"
#include <iostream>

nq::Window::Window(int width, int height, const std::string& title,
                   const Context& context, bool fullscreen = false, bool vsync = true)
                   : cached_width {width}, cached_height {height}, title {title} {
    std::cout << "\nWindow (creating)..." << std::endl;
    report_glfw(); // Gives library information, for debugging.
    glfwSetErrorCallback(error); // Will throw exception...
    glfwInit(); // Handled by error callback.

    glfwWindowHint(GLFW_SAMPLES, 4); // 4xMSAA, enable in renderer.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Why wouldn't you?!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context.major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context.minor_version);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, context.forward_compatible);
    if (context.core_profile) glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    // Errors thrown by window creation will be "handled" (heh) in the callback.

    // Note below, in order for the user to swap between contexts, a manual function is needed.
    glfwMakeContextCurrent(handle); // Meaning, last window created will have current context.
    if (fullscreen) toggle_fullscreen(); // Not really good practice or efficient...
    cached_fullscreen = fullscreen; // This is not really nice either...

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
    else glfwSwapInterval(0);
    std::cout << "...done (Window)" << std::endl;
}

// Note that glfwTerminate is not here. That needs to
// be done elsewere since it closes *all* windows, not
// only this one. Maybe create a library singleton?
nq::Window::~Window() { glfwDestroyWindow(handle); }

int nq::Window::width() const {
    if (cached_fullscreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        return mode->width;
    } else return cached_width;
}

int nq::Window::height() const {
    if (cached_fullscreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        return mode->height;
    } else return cached_height;
}

void nq::Window::toggle_fullscreen() {
    if (!cached_fullscreen) {
        cached_fullscreen = true;
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowSize(handle, mode->width, mode->height);
        glViewport(0, 0, mode->width, mode->height);
    } else {
        cached_fullscreen = false;
        resize(cached_width, cached_height);
    }
}

void nq::Window::fullscreen(bool state) {
    if (state == cached_fullscreen) return;
    else toggle_fullscreen();
}

void nq::Window::resize(int width, int height) {
    cached_width = width; cached_height = height;
    glfwSetWindowSize(handle, width, height);
    // For now we assume framebuffer == window_size.
    // Documentation at GLFW says this doesn't apply
    // everywhere (true), but glfw 3.1.2 has a bug.
    glViewport(0, 0, width, height);
}

void nq::Window::display() {
    // Render elsewhere...
    glfwSwapBuffers(handle);
    glfwPollEvents();

    // Used to print FPS count.
    ++frames; // Frames rendered.
    double current {glfwGetTime()};
    if ((current - elapsed) >= 1.0) {
        mtitle = title + " @ " + std::to_string(frames) + " FPS";
        glfwSetWindowTitle(handle, mtitle.c_str());
        elapsed = current;
        frames = 0;
    }
}

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
