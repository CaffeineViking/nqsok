#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "nqsok/window.hh"
#include "nqsok/renderer.hh"
#include "nqsok/input.hh"
#include "nqsok/shader.hh"
#include "nqsok/mesh.hh"
#include "nqsok/buffer.hh"

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
    settings.clear_color = {0x30, 0x30, 0x30};
    nq::Renderer renderer {window, settings};

    nq::Shader shader {"share/shaders/phong.vert",
                       "share/shaders/phong.frag"};

    // GLuint index_buffer;
    // glGenBuffers(1, &index_buffer);
    std::vector<GLuint> indices = {0, 1, 2};
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // GLuint vertex_buffer;
    // glGenBuffers(1, &vertex_buffer);
    std::vector<GLfloat> vertices = {0.0, 1.0,  0.0,
                                    -1.0, -1.0, 0.0,
                                    +1.0, -1.0, 0.0};
    // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glVertexAttribPointer(shader.attribute_location("position"), 3, GL_FLOAT, false, 0, nullptr);
    // glEnableVertexAttribArray(shader.attribute_location("position"));

    nq::Buffer<GLuint> index_buffer {GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW};
    nq::Buffer<GLfloat> vertex_buffer {GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW};
    nq::Mesh::Attribute position_attribute {vertex_buffer, "position"};
    nq::Mesh triangle_mesh {index_buffer, {position_attribute}};
    triangle_mesh.enable(shader);

    while (window.is_open()) {
        if (nq::Input::state(window, "close")) window.close();
        if (nq::Input::state(window, "press")) {
            std::cout << '(' << nq::Input::value(window, "xaxis") << ", "
                      << nq::Input::value(window, "yaxis") << ')' << std::endl;
        }

        renderer.clear();
        glm::mat4 model{1.0};
        model = glm::scale(model, glm::vec3{0.5});
        shader.uniform_matrix("model", model);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        window.display();
    }

    // Say goodbye to the user!
    // (only if everything went ok)
    std::cout << "\nHave a nice day!"
              << std::endl;
    return 0;
}
