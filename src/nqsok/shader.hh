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

        void uniform(const std::string&, float);
        void uniform_vector(const std::string&, const glm::vec3&);
        void uniform_matrix(const std::string&, const glm::mat4&);
        GLint attribute_location(const std::string&) const;
        void use() const { glUseProgram(handle); }

    private:
        friend class Mesh;
        friend class Renderer;
        GLuint load_shader(const std::string&, GLenum);
        GLuint load_program(GLuint, GLuint);
        GLuint handle;
    };
}

#endif
