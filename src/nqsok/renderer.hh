#ifndef NQSOK_RENDERER_HH
#define NQSOK_RENDERER_HH

#include <stdexcept>
#include "window.hh"

namespace nq {
    class Renderer_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Renderer final {
    public:
        struct Settings {
            bool depth_test {true};
            bool stencil_test {true};

            bool face_culling {true};
            GLenum front_face {GL_CCW};
            GLenum cull_face {GL_BACK};

            struct {
                char red   {0x00},
                     green {0x00},
                     blue  {0x00};
            } clear_color;
        };

        Renderer(Window&, const Settings&);
        ~Renderer(); // Releases GPU resources.

        void clear(); // Clears all relevant buffers.

    private:
        Window& window;
        Settings settings;
    };
}

#endif
