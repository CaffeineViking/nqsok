#ifndef NQSOK_MESH_HH
#define NQSOK_MESH_HH

#include <string>
#include <stdexcept>
#include <GL/glew.h>
#include "buffer.hh"

namespace nq {
    class Mesh_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Mesh final {
    public:
    private:
    };
}

#endif
