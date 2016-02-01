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

#endif
