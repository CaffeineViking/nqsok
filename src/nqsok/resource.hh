#ifndef NQSOK_RESOURCES_HH
#define NQSOK_RESOURCES_HH

#include <string>
#include <stdexcept>
#include <unordered_map>
#include "texture.hh"
#include "buffer.hh"
#include "shader.hh"
#include "image.hh"
#include "level.hh"
#include "mesh.hh"

namespace nq {
    class Resource_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Resource_manager final {
    public:
        Shader& load_shader(const std::string&, const std::string&);
        Texture& load_texture(const std::string&, const Texture::Parameters&);
        Mesh& load_mesh(const std::string&, const Level&, const Level::Data&, GLenum);
        Mesh& load_mesh(const std::string&, GLenum); // Normal mesh model loading.

    private:
        // Need better way to handle these references.
        std::unordered_map<std::string, Image> images;
        std::unordered_map<std::string, Texture> textures;
        std::unordered_map<std::string, Buffer<GLuint>> ibuffers;
        std::unordered_map<std::string, Buffer<GLfloat>> buffers;
        std::unordered_map<std::string, Shader> shaders;
        std::unordered_map<std::string, Mesh> meshes;
    };
}

#endif
