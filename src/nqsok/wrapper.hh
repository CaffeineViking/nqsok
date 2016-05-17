#ifndef NQSOK_WRAPPER_HH
#define NQSOK_WRAPPER_HH

#include<glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include "camera.hh"
#include "motion.hh"

namespace nq {
    class Camera_wrapper final {
    public:
        int get_look() const { return direction; }
        void rotate_overview(float); void rotate_reset(float);
        void rotate_left(float); void rotate_right(float);
        void update(const glm::vec3&, float);
        Camera_wrapper(Camera& handle)
                      : handle {handle} {}
    private:
        const int WRAPPING {3};
        const float OFFSET {16.0};
        const float DURATION {1.2};
        void hmotion(float); void vmotion(float);
        Motion horizontal {{0.0, glm::quarter_pi<float>()},
                           {0.0, glm::quarter_pi<float>()}, Motion::elastic_out},
               vertical   {{0.0, glm::quarter_pi<float>()},
                           {0.0, glm::quarter_pi<float>()}, Motion::elastic_out};
        glm::vec3 camera_offset {OFFSET, OFFSET, -OFFSET};
        int direction {0}, past_direction {0};
        Camera& handle;
    };
}

#endif
