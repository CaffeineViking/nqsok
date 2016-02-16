#ifndef NQSOK_MODEL_HH
#define NQSOK_MODEL_HH

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <initializer_list>
#include "mesh.hh"
#include "shader.hh"
#include "texture.hh"

namespace nq {
    class Model {
    public:
        struct Material {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            GLint shininess;
        };

        struct Sampler {
            Texture& texture;
            std::string name;
            GLint unit {-1};
        };

        Model(Mesh& mesh, Shader& shader, const Material& material,
              std::initializer_list<Sampler> samplers)
              : mesh {mesh}, shader {shader}, material {material},
                samplers {samplers.begin(), samplers.end()} {}
        Model(Mesh& mesh, Shader& shader, const Material& material)
              : Model {mesh, shader, material, {}} {}

        void scale(const glm::vec3&);
        void translate(const glm::vec3&);
        void rotate(const glm::vec3&, float);
        glm::mat4 get_transform() const { return transform; }
        void reset() { transform = glm::mat4{1.0}; }
        void apply(const std::string&);
        void append(const glm::mat4&);
        void append(const Model&);

    private:
        Mesh& mesh;
        Shader& shader;
        Material material;
        std::vector<Sampler> samplers;
        glm::mat4 transform;
        friend class Renderer;
    };
}

#endif
