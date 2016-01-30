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
        return mouse_position_x(window);
    } else if (id == "yaxis") {
        return mouse_position_y(window);
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

double nq::Input::mouse_position_x(const Window& window) {
    double value;
    glfwGetCursorPos(window.handle,
                     &value, nullptr);
    return value;
}

double nq::Input::mouse_position_y(const Window& window) {
    double value;
    glfwGetCursorPos(window.handle,
                     nullptr, &value);
    return value;
}

bool nq::Input::mouse_pressed(const Window& window, int button) {
    if (glfwGetMouseButton(window.handle, button) == GLFW_PRESS) return true;
    else return false;
}

bool nq::Input::mouse_released(const Window& window, int button) {
    if (glfwGetMouseButton(window.handle, button) == GLFW_RELEASE) return true;
    else return false;
}
