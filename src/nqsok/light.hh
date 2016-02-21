#ifndef NQSOK_LIGHT_HH
#define NQSOK_LIGHT_HH

#include <glm/glm.hpp>
#include "transform.hh"

namespace nq {
    class Light final {
    public:
        bool directional;
        Light(bool directional, const glm::vec3& position, const glm::vec3 intensity)
             : directional {directional}, position {position}, intensity {intensity} {}
        glm::vec3 position, intensity;
    };
}

#endif
