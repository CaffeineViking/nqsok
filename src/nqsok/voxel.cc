#include "voxel.hh"
#include "../extern/tinyobj.hh"

nq::Voxel::Voxel(const std::string& file) {
    std::string error;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    tinyobj::LoadObj(shapes, materials, error, file.c_str());
    if (!error.empty()) throw Voxel_error{"Voxel error (#1) " + error};

    cached_indices = shapes[0].mesh.indices;
    cached_vertices = shapes[0].mesh.positions;
    cached_normals = shapes[0].mesh.normals;
    cached_mapping = shapes[0].mesh.texcoords;
}

std::vector<GLuint> nq::Voxel::indices(GLuint offset) const {
    std::vector<GLuint> tindices {cached_indices};
    for (std::size_t i {0}; i < tindices.size(); ++i)
        tindices[i] += offset;
    return tindices;
}

std::vector<GLfloat> nq::Voxel::vertices(float x,
                                         float y,
                                         float z) const {
    std::vector<GLfloat> tvertices {cached_vertices};
    std::size_t tvertices_count {tvertices.size() / 3};
    for (std::size_t i {0}; i < tvertices_count; ++i) {
        tvertices[0 + i*3] += x;
        tvertices[1 + i*3] += y;
        tvertices[2 + i*3] += z;
    }

    return tvertices;
}

std::vector<GLfloat> nq::Voxel::colors(const Color<float>& color) const {
    std::vector<GLfloat> cached_colors;
    cached_colors.reserve(cached_vertices.size());
    std::size_t colors_count {cached_vertices.size() / 3};
    for (std::size_t i {0}; i < colors_count; ++i) {
        cached_colors.push_back(color.red);
        cached_colors.push_back(color.green);
        cached_colors.push_back(color.blue);
    }

    return cached_colors;
}
