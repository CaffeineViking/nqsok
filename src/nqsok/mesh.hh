#ifndef NQSOK_MESH_HH
#define NQSOK_MESH_HH

#include <string>
#include <stdexcept>
#include <initializer_list>
#include <unordered_map>
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
            GLuint components;
            GLboolean normalized {false};
            GLsizei stride {0};
            void* offset {nullptr};
        };

        class Builder final {
        public:
            Builder() = default;
            Builder(std::size_t elems,
                    std::size_t attribs) {
                elements.reserve(elems);
                attributes.reserve(attribs);
            }

            void merge(const std::vector<GLuint>&);
            void merge(const std::string&, const std::vector<GLfloat>&);

            template<typename F>
            void merge(const std::string& name, const std::vector<GLfloat>& data, F f) {
                std::vector<GLfloat> tattribute {f(data)};
                merge(name, tattribute);
            }

            std::size_t size() const { return elements.size(); }
            const std::vector<GLuint>& get_elements() const { return elements; }
            const std::vector<GLfloat>& get_attributes(const std::string& id) const { return attributes.at(id); }

        private:
            GLuint largest_index {0};
            std::vector<GLuint> elements; // Should suffice with only one.
            std::unordered_map<std::string, std::vector<GLfloat>> attributes;
        };

        ~Mesh() = default;
        Mesh(Buffer<GLuint>&, std::initializer_list<Attribute>);
        std::size_t size() const { return index_buffer.size(); }
        void enable(const Shader&); // With shader that is.
        bool is_current() const; // Less context swaps.

    private:
        friend class Model;
        static Mesh* current;
        Buffer<GLuint>& index_buffer;
        std::vector<Attribute> vertex_attributes;
    };
}

#endif
