#include "motion.hh"
#include<glm/gtc/constants.hpp>
#include <cmath>

float nq::Motion::bounce_out(float t) {
    if (t < 4.0 / 11.0) {
        return (121.0 * t * t) / 16.0;
    } else if (t < 8.0 / 11.0) {
        return (363.0 / 40.0 * t * t)
               - (99.0 / 10.0 * t)
               + 17.0 / 5.0;
    } else if (t < 9.0 / 10.0) {
        return (4356.0 / 361.0 * t * t)
               - (35442.0 / 1805.0 * t)
               + 16061.0 / 1805.0;
    } else {
        return (54.0 / 5.0 * t * t)
               - (513.0 / 25.0 * t)
               + 268.0 /25.0;
    }
}

float nq::Motion::elastic_out(float t) {
    return std::sin(-13.0 * glm::half_pi<float>() * (t + 1.0))
           * std::pow(2.0, -10.0 * t) + 1.0;
}

float nq::Motion::quint_inout(float t) {
    if (t < 0.5) {
        return 16.0 * std::pow(t, 5);
    } else {
        float f = 2.0 * t - 2.0;
        return 0.5 * std::pow(f, 5) + 1.0;
    }
}
