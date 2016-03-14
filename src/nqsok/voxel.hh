#ifndef NQSOK_VOXEL_HH
#define NQSOK_VOXEL_HH

#include <vector>
#include <GL/glew.h>
#include <stdexcept>
#include "color.hh"

namespace nq {
    class Voxel_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Voxel final {
    public:
        Voxel(const std::string&);
        std::vector<GLuint> indices(GLuint) const;
        std::vector<GLfloat> vertices(float, float, float) const;
        std::vector<GLfloat> colors(const Color<float>&) const;
        const std::vector<GLfloat>& normals() const { return cached_normals; }
        const std::vector<GLfloat>& mapping() const { return cached_mapping; }

    private:
        std::vector<GLuint> cached_indices;
        std::vector<GLfloat> cached_vertices;
        std::vector<GLfloat> cached_normals;
        std::vector<GLfloat> cached_mapping;
        Color<float> voxel_color {0.0, 0.0, 0.0};
    };
}

#endif
