#ifndef NQSOK_RENDERER_HH
#define NQSOK_RENDERER_HH

#include <stdexcept>
#include <glm/glm.hpp>
#include "window.hh"
#include "color.hh"
#include "model.hh"
#include "camera.hh"
#include "light.hh"

namespace nq {
    class Renderer_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Renderer final {
    public:
        // As can be seen below, these
        // are not fully platform/API independent.
        // This can be solved maybe by making the
        // Renderer class abstract, only with a few
        // basic functions that then call on concrete
        // classes, like DirectX or OpenGL to be implemented.
        struct Settings final {
            bool depth_test {false};
            bool stencil_test {false};
            bool multisampling {false};
            bool blending {false};

            bool face_culling {false};
            GLenum front_face {GL_CCW};
            GLenum cull_face {GL_BACK};
            Color<unsigned char> clear_color {0x00,
                                              0x00,
                                              0x00};
        };

        Renderer(Window&, const Settings&);
        ~Renderer(); // Releases GPU resources.

        void clear(); // Clears all relevant display buffers.
        void draw(Model&, const nq::Camera&, const std::vector<nq::Light>&);
        void draw(Model&); // User needs to handle stuff manually...

    private:
        // Maybe more reports later?
        // Possible number of triangles draw
        // every second or something like that?
        void report_settings() const;
        void setup(Model&) const;

        static constexpr double VIEWING_FRUSTRUM_NEAR {0.1};
        static constexpr double VIEWING_FRUSTRUM_FAR {100.0};

        Window& window;
        Settings settings;
        glm::mat4 projection;
    };
}

#endif
