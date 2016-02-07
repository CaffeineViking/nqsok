#ifndef NQSOK_MESH_HH
#define NQSOK_MESH_HH

#include <string>
#include <stdexcept>
#include <initializer_list>
#include <GL/glew.h>
#include "buffer.hh"
#include "shader.hh"

namespace nq {
    class Mesh_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Mesh final {
    public:
        // Currently only supports
        // GLfloat, might be able to fix
        // this so it's generic for all.
        // Later that is :)
        struct Attribute {
            Buffer<GLfloat>& buffer;
            std::string name;
            GLuint components {3};
            GLboolean normalized {false};
            GLsizei stride {0};
            void* offset {nullptr};
        };

        ~Mesh() = default;
        Mesh(Buffer<GLuint>&, std::initializer_list<Attribute>);
        std::size_t size() const { return index_buffer.size(); }
        void enable(const Shader&) const; // With shader that is.

    private:
        Buffer<GLuint>& index_buffer;
        std::vector<Attribute> vertex_attributes;
    };
}

#endif
