#ifndef NQSOK_SHADER_HH
#define NQSOK_SHADER_HH

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>

namespace nq {
    class Shader_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Shader final {
    public:
        ~Shader();
        Shader(const std::string&,
               const std::string&);

        std::string vertex_shader_file;
        std::string fragment_shader_file;
        void uniformi(const std::string&, int);
        void uniformf(const std::string&, float);
        void uniform_vector(const std::string&, const glm::vec3&);
        void uniform_matrix(const std::string&, const glm::mat4&);
        GLint attribute_location(const std::string&) const;
        void use() { glUseProgram(handle); current = this; }
        bool is_current() const;

        Shader(Shader&& other) noexcept {
            this->handle = other.handle;
            other.handle = 0;
        }

        Shader& operator=(Shader&& other) noexcept {
            this->handle = other.handle;
            other.handle = 0;
            return *this;
        }

    private:
        friend class Mesh;
        friend class Model;
        friend class Renderer;
        static Shader* current;
        GLuint load_shader(const std::string&, GLenum);
        GLuint load_program(GLuint, GLuint);
        GLuint handle;
    };
}

#endif
