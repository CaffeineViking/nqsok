#ifndef NQSOK_INPUT_HH
#define NQSOK_INPUT_HH

#include <stdexcept>
#include <unordered_map>
#include "window.hh"

namespace nq {
    class Input_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Input final {
    public:
        static bool key_pressed(int, int);
        static bool key_down(int, int);
        static bool key_up(int, int) ;

        static bool mouse_pressed(int, int);
        static bool mouse_down(int, int);
        static bool mouse_up(int, int);

        static void cursor_style(int); // GLFW_CURSOR.
        static double mouse_x(); static double mouse_y();

        static void listen(Window& window) {
            handle = window.handle;
            glfwSetKeyCallback(window.handle,
                               key_callback);
            glfwSetMouseButtonCallback(window.handle,
                                       mouse_callback);
            keyboard_state.clear();
            mouseb_state.clear();
        }

    private:
        struct Key {
            int state {GLFW_RELEASE};
            int modifier {0x0000};
            bool pressed {false};
        };

        struct Mouse {
            int state {GLFW_RELEASE};
            int modifier {0x0000};
            bool pressed {false};
        };

        static GLFWwindow* handle;
        static std::unordered_map<int, Key> keyboard_state;
        static std::unordered_map<int, Mouse> mouseb_state;
        static void key_callback(GLFWwindow*, int, int, int, int);
        static void mouse_callback(GLFWwindow*, int, int, int);
    };
}

#endif
