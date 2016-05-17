#include "wrapper.hh"
#include<glm/gtc/constants.hpp>

void nq::Camera_wrapper::turn_left() {
    pdirection = direction;
    if (direction == 0) direction = MAX_TURNS-1;
    else if (direction < MAX_TURNS) --direction;
    else return; // Don't apply if MAX_TURNS...
}

void nq::Camera_wrapper::turn_right() {
    pdirection = direction;
    if (direction == (MAX_TURNS-1)) direction = 0;
    else if (direction < MAX_TURNS) ++direction;
    else return; // Don't apply if MAX_TURNS...
}

void nq::Camera_wrapper::get_overview() {
    if (direction != MAX_TURNS)
        pdirection = direction;
    direction = MAX_TURNS;
}

void nq::Camera_wrapper::reset() {
    direction = pdirection;
}

void nq::Camera_wrapper::update(const glm::vec3& player,
                                float current_time) {
    if (direction != pdirection) {
        if (direction < MAX_TURNS) apply_hmotion(current_time);
        else apply_vmotion(current_time); // Hacky...
    }

    handle.direction = player;
    glm::vec3 rotation_offset {offsetv};
    rotation_offset.x *= std::cos(hmotion.value(current_time));
    rotation_offset.z *= std::sin(hmotion.value(current_time));
    handle.position = player + rotation_offset;
}

void nq::Camera_wrapper::apply_hmotion(float t) {
    float angle = static_cast<float>(direction)
                  * glm::half_pi<float>() + glm::quarter_pi<float>();
    hmotion.apply({t, hmotion.value(t)}, {t + MTIME, angle},
                   nq::Motion::elastic_out);
    pdirection = direction;
}

void nq::Camera_wrapper::apply_vmotion(float t) {
}
