#ifndef NQSOK_TRANSFORM_HH
#define NQSOK_TRANSFORM_HH

#include <glm/glm.hpp>

namespace nq {
    class Transform final {
    public:
        Transform() = default;
        Transform(const glm::mat4& matrix)
                 : transform {matrix} {}

        void scale(const glm::vec3&);
        void translate(const glm::vec3&);
        void rotate(const glm::vec3&, float);
        glm::mat4 get_matrix() const { return transform; }
        void reset() { transform = glm::mat4{1.0}; }
        void append(const glm::mat4&);
        void append(const Transform&);
    private: glm::mat4 transform;
    };
}

#endif
