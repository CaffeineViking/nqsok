#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "nqsok/window.hh"
#include "nqsok/renderer.hh"
#include "nqsok/input.hh"
#include "nqsok/shader.hh"
#include "nqsok/buffer.hh"
#include "nqsok/mesh.hh"

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

    std::vector<GLfloat> vertices = {
        -1.0, +1.0, +1.0,
        -1.0, -1.0, +1.0,
        +1.0, -1.0, +1.0,
        +1.0, +1.0, +1.0,

        -1.0, +1.0, -1.0,
        -1.0, -1.0, -1.0,
        +1.0, -1.0, -1.0,
        +1.0, +1.0, -1.0
    };

    nq::Buffer<GLfloat> vertex_buffer {vertices, GL_STATIC_DRAW};
    nq::Mesh::Attribute position_attribute {vertex_buffer, "position"};

    std::vector<GLfloat> normals = {
        -0.58, +0.58, +0.58,
        -0.58, -0.58, +0.58,
        +0.58, -0.58, +0.58,
        +0.58, +0.58, +0.58,

        -0.58, +0.58, -0.58,
        -0.58, -0.58, -0.58,
        +0.58, -0.58, -0.58,
        +0.58, +0.58, -0.58
    };

    nq::Buffer<GLfloat> normal_buffer {normals, GL_STATIC_DRAW};
    nq::Mesh::Attribute normal_attribute {normal_buffer, "normal"};

    std::vector<GLfloat> mappings = {
        +0.0, +0.0,
        +0.0, +1.0,
        +1.0, +1.0,
        +1.0, +0.0,

        +1.0, +0.0,
        +1.0, +1.0,
        +0.0, +1.0,
        +0.0, +0.0
    };

    nq::Buffer<GLfloat> mapping_buffer {mappings, GL_STATIC_DRAW};
    nq::Mesh::Attribute mapping_attribute {mapping_buffer, "mapping", 2};

    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 2, 3,

        4, 0, 3,
        4, 3, 7,

        1, 5, 6,
        1, 6, 2,

        4, 5, 1,
        4, 1, 0,

        3, 2, 6,
        3, 6, 7,

        7, 6, 5,
        7, 5, 4
    };

    nq::Buffer<GLuint> index_buffer {indices, GL_STATIC_DRAW};
    nq::Mesh cube_mesh {index_buffer, {position_attribute,
                                       normal_attribute,
                                       mapping_attribute}};
    cube_mesh.enable(phong_shader);

    GLfloat bwtile[] = {
        1.0, 1.0, 1.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0,
        1.0, 1.0, 1.0
    };

    GLuint bwtile_texture {42};
    glGenTextures(1, &bwtile_texture);
    glBindTexture(GL_TEXTURE_2D, bwtile_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0,
                 GL_RGB, GL_FLOAT, bwtile);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glActiveTexture(GL_TEXTURE0);
    phong_shader.uniformi("bwtile", GL_TEXTURE0);

    while (window.is_open()) {
        if (nq::Input::state(window, "close")) window.close();
        if (nq::Input::state(window, "press")) {
            std::cout << '(' << nq::Input::value(window, "xaxis") << ", "
                      << nq::Input::value(window, "yaxis") << ')' << std::endl;
        }

        renderer.clear();

        glm::mat4 view {1.0};
        phong_shader.uniform_matrix("view", view);
        glm::mat4 projection {glm::perspective(glm::half_pi<double>(), 16.0 / 9.0, 0.1, 10.0)};
        phong_shader.uniform_matrix("projection", projection);

        glm::mat4 model {1.0};
        model = glm::translate(model, glm::vec3{0.0, 0.0, -3.0});
        model = glm::rotate(model, std::sin((float)glfwGetTime()) * glm::pi<float>(), glm::vec3{0.0, 1.0, 0.0});
        model = glm::rotate(model, std::cos((float)glfwGetTime()) * glm::pi<float>(), glm::vec3{1.0, 0.0, 0.0});
        phong_shader.uniform_matrix("model", model);
        glDrawElements(GL_TRIANGLES, cube_mesh.size(), GL_UNSIGNED_INT, nullptr);

        window.display();
    }

    glDeleteTextures(1, &bwtile_texture);
    // Say goodbye to the user!
    // (only if everything went ok)
    std::cout << "\nHave a nice day!"
              << std::endl;
    return 0;
}
