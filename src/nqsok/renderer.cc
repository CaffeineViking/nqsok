#include "renderer.hh"

nq::Renderer::Renderer(Window& window, const Settings& settings)
                      : window {window}, settings {settings} {
    if (!window.has_context()) window.current_context();
    if (settings.depth_test) glEnable(GL_DEPTH_TEST);
    if (settings.stencil_test) glEnable(GL_STENCIL_TEST);
    if (settings.face_culling) {
        glEnable(GL_CULL_FACE);
        glFrontFace(settings.front_face);
        glCullFace(settings.cull_face);
    }

    glClearColor(settings.clear_color.red   / static_cast<float>(0xFF),
                 settings.clear_color.green / static_cast<float>(0xFF),
                 settings.clear_color.blue  / static_cast<float>(0xFF), 1.0f);
}

nq::Renderer::~Renderer() {  }

void nq::Renderer::clear() {
    GLbitfield mask {GL_COLOR_BUFFER_BIT};
    if (settings.depth_test) mask |= GL_DEPTH_BUFFER_BIT;
    if (settings.stencil_test) mask |= GL_STENCIL_BUFFER_BIT;
    glClear(mask); // Should clear all relevant buffers.
}
