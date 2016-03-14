#include "mesh.hh"
#include <iostream>

nq::Mesh::Mesh(Buffer<GLuint>& index_buffer,
               std::initializer_list<Attribute> attribs)
               : index_buffer {index_buffer},
                 vertex_attributes {attribs.begin(), attribs.end()} {
    std::cout << "\nMesh (binding attributes)..." << std::endl;
    std::cout << "Found element array buffer with GPU handle id number " << index_buffer.handle << std::endl;
    std::cout << "Total of " << index_buffer.size() << " vertices encountered, "
              << "assuming triangles, " << index_buffer.size() / 3 << " polygons" << std::endl;
    for (const Attribute& attribute : vertex_attributes) {
        std::cout << "Buffer with handle id " << attribute.buffer.handle
                  << " bound to '" << attribute.name << "', with "
                  << attribute.components << " GLfloat components" << std::endl;
    }

    std::cout << "...done (Mesh)" << std::endl;
}

nq::Mesh* nq::Mesh::current {nullptr};
bool nq::Mesh::is_current() const {
    if (current == this) return true;
    else return false;
}

void nq::Mesh::enable(const Shader& shader) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.handle);
    for (const Attribute& attribute : vertex_attributes) {
        glBindBuffer(GL_ARRAY_BUFFER, attribute.buffer.handle);
        glVertexAttribPointer(shader.attribute_location(attribute.name), attribute.components, GL_FLOAT,
                              attribute.normalized, attribute.stride, attribute.offset);
        glEnableVertexAttribArray(shader.attribute_location(attribute.name));
    } current = this;
}

void nq::Mesh::Builder::merge(const std::vector<GLuint>& indices) {
    GLuint largest_tindex {0};
    std::vector<GLuint> tindices {indices};
    for (std::size_t i {0}; i < tindices.size(); ++i) {
        tindices[i] += largest_index;
        if (tindices[i] > largest_tindex)
            largest_tindex = tindices[i];
    }

    largest_index = largest_tindex + 1; // Since we might be adding more.
    elements.insert(elements.end(), tindices.cbegin(), tindices.cend());
}

void nq::Mesh::Builder::merge(const std::string& name,
                              const std::vector<GLfloat>& data) {
    attributes[name].insert(attributes[name].end(),
                            data.cbegin(), data.cend());
}
