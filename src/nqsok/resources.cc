#include "resources.hh"
#include "../extern/tinyobj.hh"
#include <initializer_list>
#include "color.hh"
#include <vector>

nq::Shader& nq::Resource_manager::load_shader(const std::string& vertex_file,
                                              const std::string& fragment_file) {
    const std::string resource_identifier {vertex_file + fragment_file};
    if (shaders.find(resource_identifier) == shaders.end())
        shaders.emplace(resource_identifier, nq::Shader{vertex_file, fragment_file});

    return shaders.at(resource_identifier);
}

nq::Texture& nq::Resource_manager::load_texture(const std::string& image_file,
                                                const Texture::Parameters& parameters) {
    const std::string resource_identifier {image_file};
    if (textures.find(resource_identifier) == textures.end()) {
        const std::string image_resource_identifier {resource_identifier + "#image"};
        if (images.find(image_resource_identifier) == images.end())
            images.emplace(image_resource_identifier, nq::Image{image_file});

        nq::Image& image {images.at(image_resource_identifier)};
        textures.emplace(resource_identifier, nq::Texture{image, parameters});
    }

    return textures.at(resource_identifier);
}

nq::Mesh& nq::Resource_manager::load_mesh(const std::string& mesh_file, GLenum usage) {
    const std::string resource_identifier {mesh_file};
    if (meshes.find(resource_identifier) == meshes.end()) {
        std::string error;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        tinyobj::LoadObj(shapes, materials, error, mesh_file.c_str());
        if (!error.empty()) throw nq::Resource_error{"Resource error (#1) " + error};

        const std::string index_resource_identifier {resource_identifier + "#indices"};
        if (ibuffers.find(index_resource_identifier) == ibuffers.end())
            ibuffers.emplace(index_resource_identifier, nq::Buffer<GLuint>{shapes[0].mesh.indices, usage});

        const std::string normal_resource_identifier {resource_identifier + "#normals"};
        if (buffers.find(normal_resource_identifier) == buffers.end())
            buffers.emplace(normal_resource_identifier, nq::Buffer<GLfloat>{shapes[0].mesh.normals, usage});

        const std::string mapping_resource_identifier {resource_identifier + "#mapping"};
        if (buffers.find(mapping_resource_identifier) == buffers.end())
            buffers.emplace(mapping_resource_identifier, nq::Buffer<GLfloat>{shapes[0].mesh.texcoords, usage});

        const std::string position_resource_identifier {resource_identifier + "#positions"};
        if (buffers.find(position_resource_identifier) == buffers.end())
            buffers.emplace(position_resource_identifier, nq::Buffer<GLfloat>{shapes[0].mesh.positions, usage});

        nq::Buffer<GLuint>& index_buffer {ibuffers.at(index_resource_identifier)};
        nq::Buffer<GLfloat>& normal_buffer {buffers.at(normal_resource_identifier)};
        nq::Mesh::Attribute normal_attribute {normal_buffer, "normal", 3};

        nq::Buffer<GLfloat>& position_buffer {buffers.at(position_resource_identifier)};
        nq::Mesh::Attribute position_attribute {position_buffer, "position", 3};

        nq::Buffer<GLfloat>& mapping_buffer {buffers.at(mapping_resource_identifier)};
        nq::Mesh::Attribute mapping_attribute {mapping_buffer, "mapping", 2};

        meshes.emplace(resource_identifier, nq::Mesh{index_buffer, {normal_attribute,
                                                                    position_attribute,
                                                                    mapping_attribute}});
    }

    return meshes.at(resource_identifier);
}

nq::Mesh& nq::Resource_manager::load_mesh(const std::string& mesh_file, const Level& level,
                                          const Level::Data& level_data, GLenum usage) {
    const std::string resource_identifier {level.get_level_path()};
    if (meshes.find(resource_identifier) == meshes.end()) {
        std::string error;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        tinyobj::LoadObj(shapes, materials, error, mesh_file.c_str());
        if (!error.empty()) throw nq::Resource_error{"Resource error (#2) " + error};

        nq::Mesh::Builder mesh_builder;
        for (unsigned h {0}; h < level.get_height(); ++h) {
            for (unsigned d {0}; d < level.get_depth(); ++d) {
                for (unsigned w {0}; w < level.get_width(); ++w) {
                    nq::Color<unsigned char> voxelc {level_data[h][w+d*level.get_width()]};
                    if (voxelc == level.get_palette().empty) continue;
                    nq::Color<float> voxel_color = voxelc;
                    std::vector<GLfloat> mesh_color;
                    // We might want to make this more efficient...
                    mesh_color.reserve(shapes[0].mesh.positions.size());
                    std::size_t size {shapes[0].mesh.positions.size() / 3};
                    for (std::size_t i {0}; i < size; ++i) {
                        mesh_color.push_back(voxel_color.red);
                        mesh_color.push_back(voxel_color.green);
                        mesh_color.push_back(voxel_color.blue);
                    }

                    mesh_builder.merge(shapes[0].mesh.indices);
                    mesh_builder.merge("normal", shapes[0].mesh.normals);
                    mesh_builder.merge("mapping", shapes[0].mesh.texcoords);
                    mesh_builder.merge("color", mesh_color);
                    mesh_builder.merge("position", shapes[0].mesh.positions,
                                    [w, d, h](const std::vector<GLfloat>& v) {
                                        std::size_t size {v.size() / 3};
                                        std::vector<GLfloat> copy {v};
                                        for (std::size_t i {0}; i < size; ++i) {
                                            copy[i*3 + 0] += w*2.0f;
                                            copy[i*3 + 1] += h*2.0f;
                                            copy[i*3 + 2] += d*2.0f;
                                        }

                                        return copy;
                                    });
                }
            }
        }

        const std::string index_resource_identifier {resource_identifier + "#indices"};
        if (ibuffers.find(index_resource_identifier) == ibuffers.end())
            ibuffers.emplace(index_resource_identifier, nq::Buffer<GLuint>{mesh_builder.get_elements(), usage});

        const std::string normal_resource_identifier {resource_identifier + "#normals"};
        if (buffers.find(normal_resource_identifier) == buffers.end())
            buffers.emplace(normal_resource_identifier, nq::Buffer<GLfloat>{mesh_builder.get_attributes("normal"), usage});

        const std::string mapping_resource_identifier {resource_identifier + "#mapping"};
        if (buffers.find(mapping_resource_identifier) == buffers.end())
            buffers.emplace(mapping_resource_identifier, nq::Buffer<GLfloat>{mesh_builder.get_attributes("mapping"), usage});

        const std::string position_resource_identifier {resource_identifier + "#positions"};
        if (buffers.find(position_resource_identifier) == buffers.end())
            buffers.emplace(position_resource_identifier, nq::Buffer<GLfloat>{mesh_builder.get_attributes("position"), usage});

        const std::string color_resource_identifier {resource_identifier + "#colors"};
        if (buffers.find(color_resource_identifier) == buffers.end())
            buffers.emplace(color_resource_identifier, nq::Buffer<GLfloat>{mesh_builder.get_attributes("color"), usage});

        nq::Buffer<GLuint>& index_buffer {ibuffers.at(index_resource_identifier)};
        nq::Buffer<GLfloat>& normal_buffer {buffers.at(normal_resource_identifier)};
        nq::Mesh::Attribute normal_attribute {normal_buffer, "normal", 3};

        nq::Buffer<GLfloat>& position_buffer {buffers.at(position_resource_identifier)};
        nq::Mesh::Attribute position_attribute {position_buffer, "position", 3};

        nq::Buffer<GLfloat>& mapping_buffer {buffers.at(mapping_resource_identifier)};
        nq::Mesh::Attribute mapping_attribute {mapping_buffer, "mapping", 2};

        nq::Buffer<GLfloat>& color_buffer {buffers.at(color_resource_identifier)};
        nq::Mesh::Attribute color_attribute {color_buffer, "color", 3};

        meshes.emplace(resource_identifier, nq::Mesh{index_buffer, {normal_attribute,
                                                                    position_attribute,
                                                                    mapping_attribute,
                                                                    color_attribute}});
    }

    return meshes.at(resource_identifier);
}
