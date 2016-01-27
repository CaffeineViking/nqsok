#ifndef WRAPS_WINDOW_HH
#define WRAPS_WINDOW_HH

#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace wr {
    class Window final {
    public:
        class Error final : public std::runtime_error {
        public:     using runtime_error::runtime_error;
        };

        struct Context {
            int major;
            int minor;
            bool core;
            bool frwd;
        };

        ~Window(); // Should release everything by libs.
        Window(unsigned, unsigned, const std::string&,
               const Context&, bool = true);

        bool running() const { return !glfwWindowShouldClose(handle); }
        void display() { glfwSwapBuffers(handle); glfwPollEvents(); }

    private:
        static void input(GLFWwindow*, int, int, int, int);
        static void error(int code, const char* cmessage) {
            std::string message {"Window ("};
            message += std::to_string(code) + "): ";
            message += cmessage; // Hopefully null terminated...
            throw std::runtime_error{message};
        }

        GLFWwindow* handle;
    };
}

#endif
