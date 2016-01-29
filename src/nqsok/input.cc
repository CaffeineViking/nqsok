#include "input.hh"

bool nq::Input::state(const Window& window,
                      const std::string& id) {
    if (id == "close") {
        if (key_pressed(window, GLFW_KEY_ESCAPE)
            || key_pressed(window, GLFW_KEY_Q)) return true;
        return false;
    } else if (id == "press") {
        if (mouse_pressed(window, GLFW_MOUSE_BUTTON_LEFT)) return true;
        return false;
    } else { return false; }
}

double nq::Input::value(const Window& window,
                        const std::string& id) {
    if (id == "xaxis") {
        return mouse_position(window).x;
    } else if (id == "yaxis") {
        return mouse_position(window).y;
    } else { return 42.0; }
}

bool nq::Input::key_pressed(const Window& window, int key) {
    if (glfwGetKey(window.handle, key) == GLFW_PRESS) return true;
    else return false;
}

bool nq::Input::key_released(const Window& window, int key) {
    if (glfwGetKey(window.handle, key) == GLFW_RELEASE) return true;
    else return false;
}

void nq::Input::grab_cursor(const Window& window) {
    glfwSetInputMode(window.handle, GLFW_CURSOR,
                     GLFW_CURSOR_DISABLED);
}

void nq::Input::hide_cursor(const Window& window) {
    glfwSetInputMode(window.handle, GLFW_CURSOR,
                     GLFW_CURSOR_HIDDEN);
}

void nq::Input::normal_cursor(const Window& window) {
    glfwSetInputMode(window.handle, GLFW_CURSOR,
                     GLFW_CURSOR_NORMAL);
}

nq::Input::Position nq::Input::mouse_position(const Window& window) {
    nq::Input::Position position;
    glfwGetCursorPos(window.handle,
                     &position.x, &position.y);
    return position;
}

bool nq::Input::mouse_pressed(const Window& window, int button) {
    if (glfwGetMouseButton(window.handle, button) == GLFW_PRESS) return true;
    else return false;
}

bool nq::Input::mouse_released(const Window& window, int button) {
    if (glfwGetMouseButton(window.handle, button) == GLFW_RELEASE) return true;
    else return false;
}
