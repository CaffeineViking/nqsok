#ifndef NQSOK_IMAGE
#define NQSOK_IMAGE

#include <string>
#include <GL/glew.h>
#include <stdexcept>

namespace nq {
    class Image_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Image final {
    public:
        ~Image() { std::free(data); }
        Image(const std::string&);
        GLubyte& operator[](int i) { return data[i]; }
        GLubyte operator[](int i) const { return data[i]; }

        GLubyte* pixel_data() { return data; }
        GLsizei get_width() const { return width; }
        GLsizei get_height() const { return height; }
        GLsizei size() const { return width*height; }
        int get_bit_depth() const { return bit_depth; }
    private:
        int bit_depth;
        std::string file;
        GLsizei width, height;
        GLubyte* data {nullptr};
    };
}

#endif
