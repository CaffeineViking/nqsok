#ifndef NQSOK_WRAPPER_HH
#define NQSOK_WRAPPER_HH

#include <glm/glm.hpp>
#include "camera.hh"
#include "motion.hh"

namespace nq {
    class Camera_wrapper final {
    public:
        static constexpr int MAX_TURNS {4};
        void update(const glm::vec3&, float);
        void offset(const glm::vec3& x) { offsetv = x; }
        int dir() const { return direction; }
        void turn_left(); void turn_right();
        void get_overview(); void reset();
        Camera_wrapper(Camera& camera)
                      : handle {camera} {}

    private:
        const float MTIME {1.0};
        const float OFFSET {12.0};
        glm::vec3 offsetv {+OFFSET,
                           +OFFSET,
                           -OFFSET};
        void apply_hmotion(float); void apply_vmotion(float);
        nq::Motion hmotion {{0.0, glm::quarter_pi<float>()},
                            {0.0, glm::quarter_pi<float>()},
                            nq::Motion::elastic_out};
        int pdirection {0};
        int direction {0};
        Camera& handle;
    };
}

#endif
