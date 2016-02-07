#ifndef NQSOK_TEXTURE_HH
#define NQSOK_TEXTURE_HH

namespace nq {
    template<typename T>
    class Texture final {
    public:
        Texture(std::vector<T>&, GLsizei, GLsizei);
        void active(GLenum); // What texture unit?
    private: GLuint handle;
    };

    Texture::Texture(std::vector<T>& data, GLsizei width, GLsizei height) {
    }
}

#endif
