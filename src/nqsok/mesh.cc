#include "mesh.hh"

#include <iostream>

nq::Mesh::Mesh(Buffer<GLuint>& index_buffer,
               std::initializer_list<Attribute> attribs)
               : index_buffer {index_buffer},
                 vertex_attributes {attribs.begin(), attribs.end()} {
    std::cout << "\nMesh (binding attributes)..." << std::endl;
    std::cout << "Total of " << index_buffer.size() << " vertices encountered, "
              << "assuming triangles, " << index_buffer.size() / 3 << " polygons" << std::endl;
    for (const Attribute& attribute : vertex_attributes) {
        std::cout << "Buffer with handle id " << attribute.buffer.handle
                  << " bound to '" << attribute.name << "', with "
                  << attribute.components << " GLfloat components" << std::endl;
    }

    std::cout << "...done (Mesh)" << std::endl;
}

void nq::Mesh::enable(const Shader& shader) const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.handle);
    for (const Attribute& attribute : vertex_attributes) {
        glBindBuffer(GL_ARRAY_BUFFER, attribute.buffer.handle);
        glVertexAttribPointer(shader.attribute_location(attribute.name), attribute.components, GL_FLOAT,
                              attribute.normalized, attribute.stride, attribute.offset);
        glEnableVertexAttribArray(shader.attribute_location(attribute.name));
    }
}
