#ifndef NQSOK_CAMERA_HH
#define NQSOK_CAMERA_HH

#include <glm/glm.hpp>
#include "transform.hh"

namespace nq {
    class Camera {
    public:
        Transform transform;
        Camera(const glm::mat4& matrix)
              : transform {matrix} {}
        Camera() = default;
    };
}

#endif
