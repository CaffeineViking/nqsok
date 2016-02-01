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

    class Shader {
    public:
        ~Shader();
        Shader(const std::string&,
               const std::string&);

        void use() { glUseProgram(handle); }
        void uniform(const std::string&, float);
        void uniform_vector(const std::string&, const glm::vec4&);
        void uniform_matrix(const std::string&, const glm::mat4&);
        GLint attribute_location(const std::string&) const;

    private:
        friend class Renderer;
        GLuint load_shader(const std::string&, GLenum);
        GLuint load_program(GLuint, GLuint);
        GLuint handle;
    };
}

#endif
