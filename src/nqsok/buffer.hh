#ifndef NQSOK_BUFFER_HH
#define NQSOK_BUFFER_HH

#include <GL/glew.h>
#include <vector>

namespace nq {
    template<typename T>
    class Buffer final {
    public:
        ~Buffer(); // Deallocate memory on GPU.
        Buffer(GLenum, const std::vector<T>&, GLenum);
        std::size_t size() const { return elements; }

    private:
        friend class Mesh;
        std::size_t elements;
        GLuint handle;
    };
}

template<typename T>
nq::Buffer<T>::Buffer(GLenum type, const std::vector<T>& data, GLenum usage) {
    elements = data.size();
    glGenBuffers(1, &handle);
    glBindBuffer(type, handle);
    glBufferData(type, data.size() * sizeof(T), data.data(), usage);
}

template<typename T>
nq::Buffer<T>::~Buffer() { glDeleteBuffers(1, &handle); }

#endif
