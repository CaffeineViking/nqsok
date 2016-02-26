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
        GLsizei get_width() const { return width; } GLsizei get_height() const { return height; }
        bool is_current() const;

        Texture(Texture&& other) noexcept {
            this->width = other.width;
            this->height = other.height;
            this->handle = other.handle;
            other.handle = 0;
        }

        Texture& operator=(Texture&& other) noexcept {
            this->width = other.width;
            this->height = other.height;
            this->handle = other.handle;
            other.handle = 0;
            return *this;
        }

    private:
        static Texture* current;
        GLsizei width, height;
        GLuint handle;
    };
}

#endif
