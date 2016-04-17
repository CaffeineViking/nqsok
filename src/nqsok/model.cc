#include "model.hh"
#include <iostream>

void nq::Model::report_model() const {
    std::cout << "\nModel (instance setup)..." << std::endl;
    std::cout << "Mesh has " << mesh.size() / 3
              << " triangles" << std::endl;

    std::cout << "Mesh bound with attributes: ";
    for (const Mesh::Attribute& attribute : mesh.vertex_attributes) {
        std::cout << "'" << attribute.name << "' ";
    } std::cout << std::endl;

    std::cout << "Vertex shader used for mesh: '"
              << shader.vertex_shader_file << "'\n";
    std::cout << "Fragment shader used for mesh: '"
              << shader.fragment_shader_file << "'" << std::endl;

    std::cout << "Diffuse reflectivity for mesh: "; report_color(material.diffuse);
    std::cout << "Specular reflectivity for mesh: "; report_color(material.specular);
    std::cout << "Material exponent for mesh: " << material.shininess << std::endl;

    std::cout << "Textures bound: ";
    for (const Sampler& sampler : samplers) {
        std::cout << "('" << sampler.name
                  << "', unit #" << sampler.unit << ") ";
    } std::cout << std::endl;

    std::cout << "...done (Mesh)" << std::endl;
}

void nq::Model::report_color(const glm::vec3& color) const {
    std::cout << '(' << color.r << ", "
              << color.g << ", "
              << color.b << ')' << '\n';
}
