#ifndef NQSOK_WINDOW_HH
#define NQSOK_WINDOW_HH

#include <string>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace nq {
    class Window_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Window final {
    public:
        struct Context final {
            int major_version {2};
            int minor_version {1};
            bool core_profile {false};
            bool forward_compatible {false};
        };

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

    private:
        // Error handler for window errors.
        static void error(int, const char*);
        friend class Input; // Is part of library...
        friend class Renderer; // Handles context...

        // DANGEROUS FUNCTIONS BELOW!!!
        // Dangerous to call. Only use these in the
        // correct context. That is, before glfw is
        // initialized, before glew is initialized
        // and after an opengl context is established.

        void report_glfw() const; // Reports GLFW version and compilers flags.
        void report_glew() const; // Reports GLEW version. Needs to have glewInit.
        void report_opgl() const; // Reports GL version, renderer, vendor etc...

        std::string title; // Original title given by user. Modified below.
        std::string mtitle; // Modified title, giving extra information.
        unsigned frames {0}; // Number of frames rendered since last probe.
        double elapsed {0.0}; // Number of seconds since last fps probe.
        GLFWwindow* handle; // Window handle for interfacing with library.
    };
}

#endif
