#include "transform.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void nq::Transform::append(const glm::mat4& matrix) {
    transform *= matrix;
}

void nq::Transform::append(const Transform& other) {
    transform *= other.transform;
}

void nq::Transform::scale(const glm::vec3& amount) {
    transform = glm::scale(transform, amount);
}

void nq::Transform::translate(const glm::vec3& distance) {
    transform = glm::translate(transform, distance);
}

void nq::Transform::rotate(const glm::vec3& axis, float angle) {
    transform = glm::rotate(transform, angle, axis);
}
