#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "extern/tinyobj.hh"

#include "nqsok/window.hh"
#include "nqsok/renderer.hh"
#include "nqsok/input.hh"
#include "nqsok/shader.hh"
#include "nqsok/buffer.hh"
#include "nqsok/mesh.hh"
#include "nqsok/texture.hh"
#include "nqsok/model.hh"
#include "nqsok/camera.hh"
#include "nqsok/light.hh"
#include <GLFW/glfw3.h>

int main(int, char**) {
    nq::Window::Context context {2, 1, false, false};
    nq::Window window {1280, 720, "NQ Sokoban", context, false, true};
    // Window above has a size of 1280x720, title of 'NQ Sokoban", has
    // a context initialized to GL 2.1 (not core and not forward compat),
    // will not open in fullscreen mode and will use vertical sync (60 Hz).
    nq::Input::listen(window); // Listen to all input from this window.

    window.current_context();
    nq::Renderer::Settings settings;
    settings.depth_test = true;
    settings.face_culling = true;
    settings.multisampling = true;
    settings.clear_color = {0x30, 0x30, 0x30};
    nq::Renderer renderer {window, settings};

    nq::Shader phong_shader {"share/shaders/phong.vert",
                             "share/shaders/phong.frag"};

    std::string error;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    tinyobj::LoadObj(shapes, materials, error, "share/models/dragon.obj");
    if (!error.empty()) std::cerr << error << std::endl;

    nq::Buffer<GLuint> indices {shapes[0].mesh.indices, GL_STATIC_DRAW};
    nq::Buffer<GLfloat> vertices {shapes[0].mesh.positions, GL_STATIC_DRAW};
    nq::Mesh::Attribute position_attribute {vertices, "position", 3};
    nq::Buffer<GLfloat> normals {shapes[0].mesh.normals, GL_STATIC_DRAW};
    nq::Mesh::Attribute normal_attribute {normals, "normal", 3};
    nq::Buffer<GLfloat> texcoords {shapes[0].mesh.texcoords, GL_STATIC_DRAW};
    nq::Mesh::Attribute mapping_attribute {texcoords, "mapping", 2};
    nq::Mesh mesh {indices, {position_attribute,
                             normal_attribute,
                             mapping_attribute}};

    nq::Texture texture {"share/textures/f16.png", {GL_LINEAR, GL_LINEAR}};
    nq::Model::Sampler texture_sampler {texture, "sampler", 0};

    nq::Model::Material material {glm::vec3{0.2}, glm::vec3{0.6}, glm::vec3{0.2}, 72};
    nq::Model model {mesh, phong_shader, material, {texture_sampler}};
    nq::Camera camera {glm::vec3{0.0, 1.5, 0.0},
                       glm::vec3{0.0, 0.0, -3.0},
                       glm::vec3{0.0, 1.0, 0.0}};

    std::vector<nq::Light> lights {{true, {0.58, 0.58, 0.58}, {1.0, 1.0, 1.0}},
                                   {false, {-2.5, 0.0, -3.0}, {2.5, 0.0, 0.0}},
                                   {false, {+2.5, 0.0, -3.0}, {0.0, 0.0, 2.5}}};

    while (window.is_open()) {
        if (nq::Input::key_pressed(GLFW_KEY_Q, 0)
            || nq::Input::key_pressed(GLFW_KEY_ESCAPE, 0)) window.close();
        if (nq::Input::key_pressed(GLFW_KEY_F, 0)) window.toggle_fullscreen();

        if (nq::Input::key_down(GLFW_KEY_UP, 0)) lights[0].position.z -= 0.1;
        else if (nq::Input::key_down(GLFW_KEY_DOWN, 0)) lights[0].position.z += 0.1;
        if (nq::Input::key_down(GLFW_KEY_LEFT, 0)) lights[0].position.x -= 0.1;
        else if (nq::Input::key_down(GLFW_KEY_RIGHT, 0)) lights[0].position.x += 0.1;

        renderer.clear();
        model.transform.reset();
        model.transform.translate({0.0, 0.0, -3.0});
        model.transform.rotate({0.0, 1.0, 0.0}, glfwGetTime() / 1.0f);
        renderer.draw(model, camera, lights);
        window.display();
    }

    // Say goodbye to the user!
    // (only if everything went ok)
    std::cout << "\nHave a nice day!"
              << std::endl;
    return 0;
}
