#include "mesh.hh"

void nq::Mesh::enable(const Shader& shader) const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.handle);
    for (const Attribute& attribute : vertex_attributes) {
        glBindBuffer(GL_ARRAY_BUFFER, attribute.buffer.handle);
        glVertexAttribPointer(shader.attribute_location(attribute.name), attribute.components, GL_FLOAT,
                              attribute.normalized, attribute.stride, attribute.offset);
        glEnableVertexAttribArray(shader.attribute_location(attribute.name));
    }
}
