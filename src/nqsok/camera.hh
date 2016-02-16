#ifndef NQSOK_CAMERA_HH
#define NQSOK_CAMERA_HH

#include "transform.hh"

namespace nq {
    class Camera final {
    public:
        Camera(const glm::mat4& initial) { camera_transform.append(initial); }
        Transform& transform() { return camera_transform; }
        const Transform& transform() const { return camera_transform; }
    private: Transform camera_transform;
    };
}

#endif
