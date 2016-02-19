#ifndef NQSOK_CAMERA_HH
#define NQSOK_CAMERA_HH

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "transform.hh"

namespace nq {
    class Camera final {
    public:
        Transform transform;
        Camera(const glm::vec3& position,
               const glm::vec3& view_point,
               const glm::vec3& up_vector)
               : position {position},
                 view_point {view_point},
                 up_vector {up_vector} {}

        void move(const glm::vec3& v) { position = v; }
        void push(const glm::vec3& v) { position += v; }
        void look(const glm::vec3& v) { view_point = v; }
        void turn(const glm::vec3& v) { up_vector = v; }

        glm::vec3 get_position() const { return position; }
        glm::vec3 get_view_point() const {return view_point; }
        glm::vec3 get_up_vector() const { return up_vector; }
        glm::mat4 get_matrix() const { return glm::lookAt(position,
                                                          view_point,
                                                          up_vector); }

    private:
        glm::vec3 position;
        glm::vec3 view_point;
        glm::vec3 up_vector;
    };
}

#endif
