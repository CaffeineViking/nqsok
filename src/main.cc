#include <iostream>
#include "nqsok/window.hh"
#include "nqsok/renderer.hh"
#include "nqsok/input.hh"
#include "nqsok/shader.hh"

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

    // nq::Shader shader {"share/shaders/phong.vert",
    //                    "share/shaders/phong.frag"};

    // nq::Vertex_buffer coord_buffer {coordinates, "coordinates", 2};
    // nq::Vertex_buffer vertex_buffer {vertices, "position", 3};
    // nq::Vertex_buffer normal_buffer {normals, "normal", 3};
    // nq::Element_buffer index_buffer {indices, 3};

    // nq::Mesh mesh {index_buffer,
    //                coord_buffer,
    //                vertex_buffer,
    //                normal_buffer};
    // later... renderer.draw(mesh) (later maybe model? Shader + Mesh + Texture)
    // or.... mesh.draw(renderer) or model.draw(renderer) hmmm.... (maybe both idk)

    while (window.is_open()) {
        if (nq::Input::state(window, "close")) window.close();
        if (nq::Input::state(window, "press")) {
            std::cout << '(' << nq::Input::value(window, "xaxis") << ", "
                      << nq::Input::value(window, "yaxis") << ')' << std::endl;
        }

        renderer.clear();
        window.display();
    }

    // Say goodbye to the user!
    // (only if everything went ok)
    std::cout << "\nHave a nice day!"
              << std::endl;
    return 0;
}
