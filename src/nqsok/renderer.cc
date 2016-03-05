#include "renderer.hh"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

nq::Renderer::Renderer(Window& window, const Settings& settings)
                      : window {window}, settings {settings} {
    std::cout << "\nRenderer (initializing)..." << std::endl;
    if (!window.has_context()) window.current_context();
    if (settings.depth_test) glEnable(GL_DEPTH_TEST);
    if (settings.stencil_test) glEnable(GL_STENCIL_TEST);
    if (settings.multisampling) glEnable(GL_MULTISAMPLE);
    else glDisable(GL_MULTISAMPLE); // This one is usually enabled.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (settings.blending) glEnable(GL_BLEND);
    if (settings.face_culling) {
        glEnable(GL_CULL_FACE);
        glFrontFace(settings.front_face);
        glCullFace(settings.cull_face);
    }

    glClearColor(settings.clear_color.red   / static_cast<GLfloat>(0xFF),
                 settings.clear_color.green / static_cast<GLfloat>(0xFF),
                 settings.clear_color.blue  / static_cast<GLfloat>(0xFF), 1.0f);


    report_settings(); // Provide possibly useful information.
    std::cout << "...done (Renderer)" << std::endl;
}

nq::Renderer::~Renderer() {  }

void nq::Renderer::clear() {
    GLbitfield mask {GL_COLOR_BUFFER_BIT};
    if (settings.depth_test) mask |= GL_DEPTH_BUFFER_BIT;
    if (settings.stencil_test) mask |= GL_STENCIL_BUFFER_BIT;
    glClear(mask); // Should clear all relevant buffers.
    projection = glm::perspective(glm::radians(60.0), window.aspect_ratio(),
                                  VIEWING_FRUSTRUM_NEAR, VIEWING_FRUSTRUM_FAR);
}

void nq::Renderer::draw(Model& model) {
    setup(model);
    glDrawElements(GL_TRIANGLES, model.mesh.size(),
                   GL_UNSIGNED_INT, nullptr);
}

void nq::Renderer::draw(Model& model, const Camera& camera, const std::vector<nq::Light>& lights) {
    setup(model);
    for (std::size_t i {0}; i < 16; ++i) {
        std::string lighti {"lights[" + std::to_string(i) + "]"};
        if (i < lights.size()) model.shader.uniformi(lighti + ".enabled", 1);
        else { model.shader.uniformi(lighti + ".enabled", 0); break; }
        model.shader.uniformi(lighti + ".directional", static_cast<int>(lights[i].directional));
        model.shader.uniform_vector(lighti + ".intensity", lights[i].intensity);
        model.shader.uniform_vector(lighti + ".position", lights[i].position);
    }

    model.shader.uniform_vector("camera", camera.position);
    model.shader.uniform_matrix("view", camera.get_matrix());
    model.shader.uniform_matrix("projection", projection);
    glDrawElements(GL_TRIANGLES, model.mesh.size(),
                   GL_UNSIGNED_INT, nullptr);
}

void nq::Renderer::setup(Model& model) const {
    if (!model.shader.is_current()) model.shader.use();
    model.shader.uniform_vector("material.ambient", model.material.ambient);
    model.shader.uniform_vector("material.diffuse", model.material.diffuse);
    model.shader.uniform_vector("material.specular", model.material.specular);
    model.shader.uniformi("material.shininess", model.material.shininess);
    for (Model::Sampler& sampler : model.samplers) {
        if (!sampler.texture.is_current()) {
            sampler.texture.active(model.shader,
                    sampler.unit, sampler.name);
        }
    }

    model.apply("model"); // Apply model matrix transform.
    if (!model.mesh.is_current()) model.mesh.enable(model.shader);
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
