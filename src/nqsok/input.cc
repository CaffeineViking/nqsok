#include "input.hh"
#include <iostream>

bool nq::Input::key_pressed(int key, int modifier) {
    if (key_down(key, modifier) && keyboard_state[key].pressed) {
        keyboard_state[key].pressed = false;
        return true;
    }

    keyboard_state[key].pressed = false;
    return false;
}

bool nq::Input::key_down(int key, int modifier) {
    if (keyboard_state.count(key) == 0) return false;
    if ((keyboard_state[key].state == GLFW_PRESS || keyboard_state[key].state == GLFW_REPEAT)
        && keyboard_state[key].modifier == modifier) return true;
    else return false;
}

bool nq::Input::key_up(int key, int modifier) {
    if (keyboard_state.count(key) == 0) return true;
    if (keyboard_state[key].state == GLFW_RELEASE
        && keyboard_state[key].modifier == modifier) return true;
    else return false;
}

GLFWwindow* nq::Input::handle;
std::unordered_map<int, nq::Input::Key> nq::Input::keyboard_state;
void nq::Input::key_callback(GLFWwindow*, int key,
                             int, int action, int mods) {
    if (keyboard_state.count(key) == 0) keyboard_state[key] = {};
    if (keyboard_state[key].state == GLFW_RELEASE
        && action == GLFW_PRESS) keyboard_state[key].pressed = true;
    else keyboard_state[key].pressed = false;
    keyboard_state[key].state = action;
    keyboard_state[key].modifier = mods;
}

bool nq::Input::mouse_pressed(int button, int modifier) {
    if (mouse_down(button, modifier) && mouseb_state[button].pressed) {
        mouseb_state[button].pressed = false;
        return true;
    }

    mouseb_state[button].pressed = false;
    return false;
}

bool nq::Input::mouse_down(int button, int modifier) {
    if (mouseb_state.count(button) == 0) return false;
    if (mouseb_state[button].state == GLFW_PRESS
        && mouseb_state[button].modifier == modifier) return true;
    else return false;
}

bool nq::Input::mouse_up(int button, int modifier) {
    if (mouseb_state.count(button) == 0) return true;
    if (mouseb_state[button].state == GLFW_RELEASE
        && mouseb_state[button].modifier == modifier) return true;
    else return false;
}

std::unordered_map<int, nq::Input::Mouse> nq::Input::mouseb_state;
void nq::Input::mouse_callback(GLFWwindow*, int button,
                               int action, int mods) {
    if (mouseb_state.count(button) == 0) mouseb_state[button] = {};
    if (mouseb_state[button].state == GLFW_RELEASE
        && action == GLFW_PRESS) mouseb_state[button].pressed = true;
    else mouseb_state[button].pressed = false;
    mouseb_state[button].state = action;
    mouseb_state[button].modifier = mods;
}

void nq::Input::cursor_style(int value) { glfwSetInputMode(handle, GLFW_CURSOR, value); }
double nq::Input::mouse_x() {
    double value;
    glfwGetCursorPos(handle, &value,
                     nullptr);
    return value;
}

double nq::Input::mouse_y() {
    double value;
    glfwGetCursorPos(handle, nullptr,
                     &value);
    return value;
}
