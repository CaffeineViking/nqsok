#ifndef NQSOK_WINDOW_HH
#define NQSOK_WINDOW_HH

#include <string>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "context.hh"

namespace nq {
    class Window_error : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Window {
    public:
        // Constructor creates a window and initializes
        // the context with the provided settings. Parameters
        // that must be provided are 'width' and 'height' of the
        // window, 'title' in a string, if it should be 'fullscreen
        // on start-up and if the buffer should do 'vsync' on frame.
        Window(int, int, const std::string&, const Context&, bool, bool);
        ~Window(); // Released resouces allocated by windowing library.

        // is_open(), check is window is still open.
        // current(), swaps context to this.

    private:
        static void error(int, const char*);
        GLFWwindow* handle;
    };
}

#endif
