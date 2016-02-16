#ifndef NQSOK_LIGHT_HH
#define NQSOK_LIGHT_HH

#include <glm/glm.hpp>
#include "transform.hh"

namespace nq {
    class Light final {
    public:
        Light(bool directional, const glm::vec4& position, const glm::vec3 intensity)
             : directional {directional}, position {position}, intensity {intensity} {}
        glm::vec4 get_position() const { return light_transform.get_matrix() * position; }
        glm::vec3 get_intensity() const { return intensity; }
        bool is_directional() const { return directional; }
        Transform& transform() { return light_transform; }
        const Transform& transform() const { return light_transform; }

    private:
        bool directional;
        glm::vec4 position;
        glm::vec3 intensity;
        Transform light_transform;
    };
}

#endif
