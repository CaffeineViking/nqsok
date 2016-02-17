#ifndef NQSOK_MODEL_HH
#define NQSOK_MODEL_HH

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>
#include <initializer_list>

#include "mesh.hh"
#include "shader.hh"
#include "texture.hh"
#include "transform.hh"

namespace nq {
    class Model {
    public:
        struct Material final {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            GLint shininess;
        };

        struct Sampler final {
            Texture& texture;
            std::string name;
            GLint unit;
        };

        Model(Mesh& mesh, Shader& shader, const Material& material,
              std::initializer_list<Sampler> samplers)
              : mesh {mesh}, shader {shader}, material {material},
                samplers {samplers.begin(), samplers.end()} {}
        Model(Mesh& mesh, Shader& shader, const Material& material)
              : Model {mesh, shader, material, {}} {}

        Transform transform; // Shhhh...
        void apply(const std::string& uniform) {
            shader.uniform_matrix(uniform, transform.get_matrix());
        }

    private:
        Mesh& mesh;
        Shader& shader;
        Material material;
        std::vector<Sampler> samplers;
        friend class Renderer;
    };
}

#endif
