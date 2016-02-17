#ifndef NQSOK_CAMERA_HH
#define NQSOK_CAMERA_HH

#include "transform.hh"

namespace nq {
    class Camera final {
    public:
        Transform transform;
        Camera(const glm::mat4& initial) { transform.append(initial); }
        Camera() = default;
    };
}

#endif
