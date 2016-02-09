#include "model.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void nq::Model::apply(const std::string& uniform) {
    shader.uniform_matrix(uniform, transform);
    transform = glm::mat4{1.0}; // Reset matrix.
}

void nq::Model::append(const glm::mat4& matrix) {
    transform *= matrix;
}

void nq::Model::append(const Model& model) {
    transform *= model.transform;
}

void nq::Model::scale(const glm::vec3& amount) {
    transform = glm::scale(transform, amount);
}

void nq::Model::translate(const glm::vec3& distance) {
    transform = glm::translate(transform, distance);
}

void nq::Model::rotate(const glm::vec3& axis, float angle) {
    transform = glm::rotate(transform, angle, axis);
}
