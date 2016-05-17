#ifndef NQSOK_WRAPPER_HH
#define NQSOK_WRAPPER_HH

#include <glm/glm.hpp>
#include "camera.hh"

namespace nq {
    class Camera_wrapper final {
    public:
        static constexpr int MAX_TURNS {4};
        void update(const glm::vec3&, float);
        void offset(float x) { offsetv = x; }
        int dir() const { return direction; }
        void turn_left(); void turn_right();
        void get_overview(); void reset();
        Camera_wrapper(Camera& camera)
                      : target {camera.position},
                        position {camera.direction},
                        handle {camera} {}

    private:
        glm::vec3 target;
        glm::vec3 position;
        float offsetv {12.0};
        int pdirection {0};
        int direction {0};
        Camera& handle;
    };
}

#endif
