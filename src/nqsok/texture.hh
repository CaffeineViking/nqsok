#ifndef NQSOK_TEXTURE_HH
#define NQSOK_TEXTURE_HH

#include <vector>
#include <string>
#include <GL/glew.h>
#include "shader.hh"

namespace nq {
    class Texture_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Texture final {
    public:
        struct Parameters {
            GLenum minifying_filter {GL_NEAREST};
            GLenum magnifying_filter {GL_NEAREST};
        };

        ~Texture() { glDeleteTextures(1, &handle); }
        Texture(const std::string&, const Parameters&);
        Texture(std::vector<GLfloat>&, GLsizei, GLsizei, const Parameters&);
        void active(Shader&, GLenum, const std::string&); // What texture unit and name?

    private:
        GLsizei width, height;
        GLuint handle;
    };
}

#endif
