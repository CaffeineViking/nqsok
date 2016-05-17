#include "wrapper.hh"

void nq::Camera_wrapper::turn_left() {
    int previous_direction = direction;
    if (direction == 0) direction = MAX_TURNS-1;
    else if (direction < MAX_TURNS) --direction;
    else return; // Don't apply if MAX_TURNS...
    pdirection = previous_direction;
}

void nq::Camera_wrapper::turn_right() {
    int previous_direction = direction;
    if (direction == (MAX_TURNS-1)) direction = 0;
    else if (direction < MAX_TURNS) ++direction;
    else return; // Don't apply if MAX_TURNS...
    pdirection = previous_direction;
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
}
