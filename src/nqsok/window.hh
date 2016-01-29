#ifndef NQSOK_WINDOW_HH
#define NQSOK_WINDOW_HH

#include <string>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "context.hh"

namespace nq {
    class Window_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Window final {
    public:
        // Constructor creates a window and initializes
        // the context with the provided settings. Parameters
        // that must be provided are 'width' and 'height' of the
        // window, 'title' in a string, if it should be 'fullscreen
        // on start-up and if the buffer should do 'vsync' on frame.
        Window(int, int, const std::string&, const Context&, bool, bool);
        ~Window(); // Released resouces allocated by windowing library.

        // Check if the window is still open. Library defined.
        bool is_open() const { return !glfwWindowShouldClose(handle); }
        void close() { glfwSetWindowShouldClose(handle, true); }

        // Marks this window as having the current context.
        void current_context() { glfwMakeContextCurrent(handle); }
        bool has_context() const { return (handle == glfwGetCurrentContext()); }

        void display() {
            // Render elsewhere...
            glfwSwapBuffers(handle);
            glfwPollEvents();
        }

    private:
        // Error handler for window errors.
        static void error(int, const char*);
        friend class Input; // Is part of library...

        // DANGEROUS FUNCTIONS BELOW!!!
        // Dangerous to call. Only use these in the
        // correct context. That is, before glfw is
        // initialized, before glew is initialized
        // and after an opengl context is established.

        void report_glfw() const;
        void report_glew() const;
        void report_opgl() const;
        GLFWwindow* handle;
    };
}

#endif
