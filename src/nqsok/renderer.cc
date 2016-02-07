#include "renderer.hh"
#include <iostream>

nq::Renderer::Renderer(Window& window, const Settings& settings)
                      : window {window}, settings {settings} {
    std::cout << "\nRenderer (initializing)..." << std::endl;
    if (!window.has_context()) window.current_context();
    if (settings.depth_test) glEnable(GL_DEPTH_TEST);
    if (settings.stencil_test) glEnable(GL_STENCIL_TEST);
    if (settings.multisampling) glEnable(GL_MULTISAMPLE);
    else glDisable(GL_MULTISAMPLE); // This one is usually enabled.
    if (settings.face_culling) {
        glEnable(GL_CULL_FACE);
        glFrontFace(settings.front_face);
        glCullFace(settings.cull_face);
    }

    glClearColor(settings.clear_color.red   / static_cast<GLchar>(0xFF),
                 settings.clear_color.green / static_cast<GLchar>(0xFF),
                 settings.clear_color.blue  / static_cast<GLchar>(0xFF), 1.0f);


    report_settings(); // Provide possibly useful information.
    std::cout << "...done (Renderer)" << std::endl;
}

nq::Renderer::~Renderer() {  }

void nq::Renderer::clear() {
    GLbitfield mask {GL_COLOR_BUFFER_BIT};
    if (settings.depth_test) mask |= GL_DEPTH_BUFFER_BIT;
    if (settings.stencil_test) mask |= GL_STENCIL_BUFFER_BIT;
    glClear(mask); // Should clear all relevant buffers.
}

void nq::Renderer::report_settings() const {
    if (settings.depth_test) std::cout << "Depth test enabled" << std::endl;
    if (settings.stencil_test) std::cout << "Stencil test enabled" << std::endl;
    if (settings.multisampling) std::cout << "Multisampling enabled" << std::endl;
    if (settings.face_culling) {
        std::cout << "Face culling enabled" << std::endl;
        if (settings.front_face == GL_CW) std::cout << "Front face is clockwise" << std::endl;
        else if (settings.front_face == GL_CCW) std::cout << "Front face is counter clockwise" << std::endl;
        if (settings.cull_face == GL_FRONT) std::cout << "Culling polygon's front faces" << std::endl;
        if (settings.cull_face == GL_BACK) std::cout << "Culling polygon's back faces" << std::endl;
    }
}
