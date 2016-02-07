#ifndef NQSOK_BUFFER_HH
#define NQSOK_BUFFER_HH

#include <GL/glew.h>
#include <vector>

namespace nq {
    template<typename T>
    class Buffer final {};

    template<>
    class Buffer<GLfloat> final {
    public:
        ~Buffer() { glDeleteBuffers(1, &handle); } // Deallocate memory on GPU.
        std::size_t size() const { return elements; }
        Buffer(const std::vector<GLfloat>& data, GLenum usage) {
            elements = data.size();
            glGenBuffers(1, &handle);
            glBindBuffer(GL_ARRAY_BUFFER, handle);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), usage);
        }

    private:
        friend class Mesh;
        std::size_t elements;
        GLuint handle;
    };

    template<>
    class Buffer<GLuint> final {
    public:
        ~Buffer() { glDeleteBuffers(1, &handle); } // Deallocate memory on GPU.
        std::size_t size() const { return elements; }
        Buffer(const std::vector<GLuint>& data, GLenum usage) {
            elements = data.size();
            glGenBuffers(1, &handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), usage);
        }

    private:
        friend class Mesh;
        std::size_t elements;
        GLuint handle;
    };
}

#endif
