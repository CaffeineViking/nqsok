#include "nqsok/window.hh"
#include "nqsok/context.hh"

int main(int, char**) {
    nq::Context context {2, 1, false, false};
    nq::Window window {1280, 720, "NQ Sokoban", context, false, true};
    window.current_context();

    while (window.is_open()) {
        window.display();
    }

    return 0;
}
