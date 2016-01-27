#include "wraps/window.hh"
int main(int, char**) {
    wr::Window::Context context {2, 1, false, false};
    wr::Window window {1280, 720, "NQ Sokoban", context};
    while (window.running()) {
        // Update here...
        // Render here...
        window.display();
    }

    return 0;
}
