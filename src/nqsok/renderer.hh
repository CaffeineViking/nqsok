#ifndef NQSOK_RENDERER_HH
#define NQSOK_RENDERER_HH

#include <stdexcept>
#include "window.hh"
#include "shader.hh"
#include "color.hh"

namespace nq {
    class Renderer_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Renderer final {
    public:
        struct Settings final {
            bool depth_test {false};
            bool stencil_test {false};
            bool multisampling {false};

            bool face_culling {false};
            GLenum front_face {GL_CCW};
            GLenum cull_face {GL_BACK};

            Color<char> clear_color {0x00,
                                     0x00,
                                     0x00};
        };

        Renderer(Window&, const Settings&);
        ~Renderer(); // Releases GPU resources.

        void clear(); // Clears all relevant display buffers.

    private:
        // Maybe more reports later?
        void report_settings() const;

        Window& window;
        Settings settings;
    };
}

#endif
