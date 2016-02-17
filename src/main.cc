#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include "extern/tiny_obj_loader.h"

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

int main(int, char**) {
    nq::Window::Context context {2, 1, false, false};
    nq::Window window {1280, 720, "NQ Sokoban", context, false, true};
    // Window above has a size of 1280x720, title of 'NQ Sokoban", has
    // a context initialized to GL 2.1 (not core and not forward compat),
    // will not open in fullscreen mode and will use vertical sync (60 Hz).

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
    tinyobj::LoadObj(shapes, materials, error, "share/models/bunny.obj");
    if (!error.empty()) std::cerr << error << std::endl;

    nq::Buffer<GLuint> indices {shapes[0].mesh.indices, GL_STATIC_DRAW};
    nq::Buffer<GLfloat> vertices {shapes[0].mesh.positions, GL_STATIC_DRAW};
    nq::Mesh::Attribute position_attribute {vertices, "position", 3};
    nq::Buffer<GLfloat> normals {shapes[0].mesh.normals, GL_STATIC_DRAW};
    nq::Mesh::Attribute normal_attribute {normals, "normal", 3};
    nq::Mesh mesh {indices, {position_attribute, normal_attribute}};

    nq::Model::Material material {glm::vec3{0.2}, glm::vec3{0.6}, glm::vec3{0.2}, 70};
    nq::Model model {mesh, phong_shader, material};
    nq::Camera camera {glm::lookAt(glm::vec3{0.0, 1.0, 0.0},
                                   glm::vec3{0.0, 0.0, -5.0},
                                   glm::vec3{0.0, 1.0, 0.0})};

    std::vector<nq::Light> lights {{false, {0.58, 0.58, 0.58}, {3.0, 3.0, 3.0}}};

    while (window.is_open()) {
        if (nq::Input::state(window, "close")) window.close();
        if (nq::Input::state(window, "fullscreen")) window.fullscreen(true);

        if (nq::Input::state(window, "up")) {
            lights[0].push({0.0, 0.0, -0.1});
        } else if (nq::Input::state(window, "down")) {
            lights[0].push({0.0, 0.0, +0.1});
        }

        if (nq::Input::state(window, "left")) {
            lights[0].push({-0.1, 0.0, 0.0});
        } else if (nq::Input::state(window, "right")) {
            lights[0].push({+0.1, 0.0, 0.0});
        }


        renderer.clear();
        model.transform.reset();
        model.transform.translate({0.0, 0.0, -5.0});
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
