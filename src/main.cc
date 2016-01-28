#include "nqsok/window.hh"
#include "nqsok/context.hh"
#include "nqsok/input.hh"

int main(int, char**) {
    nq::Context context {2, 1, false, false};
    nq::Window window {1280, 720, "NQ Sokoban", context, false, true};
    // Window above has a size of 1280x720, title of 'NQ Sokoban", has
    // a context initialized to GL 2.1 (not core and not forward compat),
    // will not open in fullscreen mode and will use vertical sync (60 Hz).

    window.current_context();
    while (window.is_open()) {
        window.display();
    }

    return 0;
}
