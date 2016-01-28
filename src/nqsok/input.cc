#include "input.hh"

bool nq::Input::key_pressed(const Window& window, int key) {
    if (glfwGetKey(window.handle, key) == GLFW_PRESS) return true;
    else return false;
}

bool nq::Input::key_released(const Window& window, int key) {
    if (glfwGetKey(window.handle, key) == GLFW_RELEASE) return true;
    else return false;
}

nq::Input::Position nq::Input::mouse_position(const Window& window) {
    nq::Input::Position position;
    glfwGetCursorPos(window.handle,
                     &position.x, &position.y);
    return position;
}
