#include "nqsok/window.hh"
#include "nqsok/context.hh"

int main(int, char**) {
    nq::Context context {2, 1, false, false};
    nq::Window window {1280, 720, "NQ Sokoban", context, false, true};
    glfwTerminate(); // Library wrapper doesn't handle this yet...
    return 0;
}
