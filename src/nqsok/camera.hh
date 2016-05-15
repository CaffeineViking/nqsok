#ifndef NQSOK_CAMERA_HH
#define NQSOK_CAMERA_HH

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "transform.hh"

namespace nq {
    class Camera {
    public:
        glm::vec3 position,
                  direction, up;

        glm::mat4 get_matrix() const {
            return glm::lookAt(position,
                               direction, up);
        }

        static constexpr float OFFSET {12.0};
        Camera(const glm::vec3& position,
               const glm::vec3& direction,
               const glm::vec3& up_vector)
               : position {position},
                 direction {direction},
                 up {up_vector} {}
    };
}

#endif
