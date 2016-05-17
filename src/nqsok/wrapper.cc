#include "wrapper.hh"
#include <iostream>
#include <cmath>

void nq::Camera_wrapper::update(const glm::vec3& player_position,
                                float current_time) {
    handle.direction = player_position;
    glm::vec3 hoffset = camera_offset;
    hoffset.x *= std::cos(horizontal.value(current_time));
    hoffset.z *= std::sin(horizontal.value(current_time));
    handle.position  = player_position + hoffset;
}

void nq::Camera_wrapper::vmotion(float t) {
}

void nq::Camera_wrapper::rotate_reset(float t) {
    int temporary  {direction};
    direction = past_direction;
    past_direction = temporary;
}

void nq::Camera_wrapper::rotate_overview(float t) {
    if (direction != (WRAPPING + 1))
        past_direction = direction;
    direction = (WRAPPING + 1);
}

void nq::Camera_wrapper::rotate_left(float t) {
    int temporary_direction {direction};
    if (direction == 0) direction = WRAPPING;
    else if (direction <= WRAPPING) --direction;
    else if (direction == (WRAPPING + 1)) return;
    past_direction = temporary_direction;
    hmotion(t); // Apply nq::Motion...
}

void nq::Camera_wrapper::rotate_right(float t) {
    int temporary_direction {direction};
    if (direction == WRAPPING) direction = 0;
    else if (direction <= WRAPPING) ++direction;
    else if (direction == (WRAPPING + 1)) return;
    past_direction = temporary_direction;
    hmotion(t); // Apply nq::Motion...
}

void nq::Camera_wrapper::hmotion(float t) {
    float future_angle {glm::quarter_pi<float>()};
    nq::Motion::Point current {t, horizontal.value(t)};
    if (direction == 0 && past_direction == WRAPPING)
        current = {t, horizontal.value(t)-glm::two_pi<float>()};
    if (direction == WRAPPING && past_direction == 0)
        current = {t, horizontal.value(t)+glm::two_pi<float>()};
    future_angle += glm::half_pi<float>() * direction;
    nq::Motion::Point next {t + DURATION, future_angle};
    horizontal.apply(current, next, nq::Motion::elastic_out);
}
