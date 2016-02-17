#ifndef NQSOK_LIGHT_HH
#define NQSOK_LIGHT_HH

#include <glm/glm.hpp>
#include "transform.hh"

namespace nq {
    class Light final {
    public:
        Light(bool directional, const glm::vec3& position, const glm::vec3 intensity)
             : directional {directional}, position {position}, intensity {intensity} {}
        void move(const glm::vec3& position) { this->position = position; }
        void push(const glm::vec3& direction) { this->position += direction; }
        void color(const glm::vec3& color) { this->intensity = color; }
        glm::vec3 get_position() const { return position; }
        glm::vec3 get_intensity() const { return intensity; }
        bool is_directional() const { return directional; }

    private:
        bool directional;
        glm::vec3 position;
        glm::vec3 intensity;
    };
}

#endif
