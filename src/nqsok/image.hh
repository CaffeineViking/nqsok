#ifndef NQSOK_IMAGE_HH
#define NQSOK_IMAGE_HH

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

        Image(Image&& other) noexcept {
            this->bit_depth = other.bit_depth;
            this->file = other.file;
            this->width = other.width;
            this->height = other.height;
            this->data = other.data;
            other.data = nullptr;
            other.file = "MOVED";
        }

        Image& operator=(Image&& other) noexcept {
            this->bit_depth = other.bit_depth;
            this->file = other.file;
            this->width = other.width;
            this->height = other.height;
            this->data = other.data;
            other.data = nullptr;
            other.file = "MOVED";
            return *this;
        }
    private:
        int bit_depth;
        std::string file;
        GLsizei width, height;
        GLubyte* data {nullptr};
    };
}

#endif
