#include <queue>
#include <vector>
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "nqsok/window.hh"
#include "nqsok/renderer.hh"
#include "nqsok/input.hh"

#include "nqsok/shader.hh"
#include "nqsok/buffer.hh"
#include "nqsok/mesh.hh"
#include "nqsok/texture.hh"
#include "nqsok/resource.hh"
#include "nqsok/wrapper.hh"

#include "nqsok/model.hh"
#include "nqsok/camera.hh"
#include "nqsok/motion.hh"
#include "nqsok/light.hh"

#include "nqsok/root.hh"
#include "nqsok/pack.hh"
#include "nqsok/level.hh"
#include "nqsok/score.hh"
#include "nqsok/sokoban.hh"
#include <GLFW/glfw3.h>

#ifndef SHARE
#define SHARE "share/"
#endif

enum class Argument {
    NONE, ROOT,
    LEVEL, PACK,
    HELP_NEEDED
};

static std::string share {SHARE};
static std::string version {"1.0D"};
static std::string rootf {share + "packs.nqr"};
std::queue<nq::Level> load(Argument, char**);
Argument pargs(int, char**);
void help(const char*);

int main(int argc, char** argv) {
    std::queue<nq::Level> levels;
    Argument type {pargs(argc, argv)};
    if (type == Argument::HELP_NEEDED
        || type == Argument::NONE) {
        help(argv[0]);
        return 0;
    } else levels = load(type, argv);
    nq::Score score {"sokoban.nqh"};

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

    nq::Level& level {levels.front()}; // Load first level on queue.
    nq::Level::Data level_data {level.data(level.get_path_parent())};
    // Above operation is quite expensive, only do this once kay...
    nq::Sokoban sokoban {level, level_data}; // Game itself.
    nq::Resource_manager rm; // Quite a shitty solution...
    renderer.clear_color(level.get_palette().background);

    nq::Mesh& voxel_mesh {rm.load_mesh(share + "models/voxel.obj", GL_STATIC_DRAW)};
    nq::Mesh& svoxel_mesh {rm.load_mesh(share + "models/svoxel.obj", GL_STATIC_DRAW)};
    nq::Mesh& level_mesh {rm.load_mesh(share + "models/voxel.obj", level, level_data, GL_STATIC_DRAW)};

    nq::Texture::Parameters surface_texture_parameters {GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR};
    nq::Texture& surface_texture {rm.load_texture(share + "textures/surface.png", surface_texture_parameters)};
    nq::Shader& aphong_shader {rm.load_shader(share + "shaders/aphong.vert", share + "shaders/aphong.frag")};
    nq::Shader& uphong_shader {rm.load_shader(share + "shaders/uphong.vert", share + "shaders/uphong.frag")};

    nq::Model::Sampler level_model_sampler {surface_texture, "sampler", 0};
    nq::Model::Material level_model_material {glm::vec3{0.8}, glm::vec3{0.4}, 42};
    nq::Model level_model {level_mesh, aphong_shader, level_model_material, {level_model_sampler}};

    nq::Model::Sampler voxel_model_sampler {surface_texture, "sampler", 0};
    nq::Model::Material voxel_model_material {glm::vec3{0.8}, glm::vec3{0.4}, 42};
    nq::Model voxel_model {voxel_mesh, uphong_shader, voxel_model_material, {voxel_model_sampler}};
    nq::Model svoxel_model {svoxel_mesh, uphong_shader, voxel_model_material, {voxel_model_sampler}};

    nq::Camera camera {glm::vec3{+0.0, +0.0, +0.0},
                       glm::vec3{+0.0, +0.0, +0.0},
                       glm::vec3{+0.0, +1.0, +0.0}};
    nq::Camera_wrapper camera_wrapper {camera}; // Motions...
    std::vector<nq::Light> lights {{true, {0.58, 0.58, 0.58},
                                          {1.00, 1.00, 1.00}}};

    double itime {glfwGetTime()};
    double cached_time {itime};
    while (window.is_open()) {
        if (nq::Input::key_pressed(GLFW_KEY_Q, 0)
            || nq::Input::key_pressed(GLFW_KEY_ESCAPE, 0)) window.close();
        if (nq::Input::key_pressed(GLFW_KEY_F, 0)) window.toggle_fullscreen();

        if (nq::Input::key_pressed(GLFW_KEY_R, 0))
            sokoban.reset(); // Restart the level.
        if (nq::Input::key_pressed(GLFW_KEY_U, 0))
            sokoban.undo(); // Undo a last action.

        if (nq::Input::key_pressed(GLFW_KEY_UP, 0)
            || nq::Input::key_pressed(GLFW_KEY_K, 0))
            sokoban.step(nq::Sokoban::Action::FORWARD);
        else if (nq::Input::key_pressed(GLFW_KEY_DOWN, 0)
                 || nq::Input::key_pressed(GLFW_KEY_J, 0))
            sokoban.step(nq::Sokoban::Action::BACKWARD);
        if (nq::Input::key_pressed(GLFW_KEY_LEFT, 0)
            || nq::Input::key_pressed(GLFW_KEY_H, 0))
            sokoban.step(nq::Sokoban::Action::RIGHT);
        else if (nq::Input::key_pressed(GLFW_KEY_RIGHT, 0)
                 || nq::Input::key_pressed(GLFW_KEY_L, 0))
            sokoban.step(nq::Sokoban::Action::LEFT);


        double current_time {glfwGetTime()};
        double delta_time {current_time - cached_time};
        float current_float_time = current_time;
        cached_time = current_time;

        if (nq::Input::key_pressed(GLFW_KEY_UP, GLFW_MOD_SHIFT)
            || nq::Input::key_pressed(GLFW_KEY_K, GLFW_MOD_SHIFT))
            camera_wrapper.rotate_overview(current_float_time);
        else if (nq::Input::key_pressed(GLFW_KEY_DOWN, GLFW_MOD_SHIFT)
                 || nq::Input::key_pressed(GLFW_KEY_J, GLFW_MOD_SHIFT))
            camera_wrapper.rotate_reset(current_float_time);
        if (nq::Input::key_pressed(GLFW_KEY_LEFT, GLFW_MOD_SHIFT)
            || nq::Input::key_pressed(GLFW_KEY_H, GLFW_MOD_SHIFT))
            camera_wrapper.rotate_left(current_float_time);
        else if (nq::Input::key_pressed(GLFW_KEY_RIGHT, GLFW_MOD_SHIFT)
                 || nq::Input::key_pressed(GLFW_KEY_L, GLFW_MOD_SHIFT))
            camera_wrapper.rotate_right(current_float_time);
        glm::vec3 player_position {glm::vec3{sokoban.get_player()}
                                   * nq::Level::VOXEL_SIZE};
        camera_wrapper.update(player_position, current_float_time);

        renderer.clear();
        level_model.transform.reset();
        level_model.transform.translate({0.0, 0.0, 0.0});
        renderer.draw(level_model, camera, lights);

        for (const nq::Sokoban::Position& moveable : sokoban.get_moveables()) {
            glm::vec3 position {glm::vec3{moveable} * nq::Level::VOXEL_SIZE};
            voxel_model.transform.reset(); voxel_model.transform.translate(position);
            if (sokoban.objective(moveable)) uphong_shader.uniform_vector("color", level.get_palette().objective);
            else uphong_shader.uniform_vector("color", level.get_palette().moveable);
            renderer.draw(voxel_model, camera, lights);
        }

        for (const nq::Sokoban::Position& objective : sokoban.get_objectives()) {
            glm::vec3 position {glm::vec3{objective} * nq::Level::VOXEL_SIZE};
            svoxel_model.transform.reset(); svoxel_model.transform.translate(position);
            uphong_shader.uniform_vector("color", level.get_palette().moveable);
            renderer.draw(svoxel_model, camera, lights);
        }

        glm::vec3 position {glm::vec3{sokoban.get_player()} * nq::Level::VOXEL_SIZE};
        voxel_model.transform.reset(); voxel_model.transform.translate(position);
        uphong_shader.uniform_vector("color", level.get_palette().player);
        renderer.draw(voxel_model, camera, lights);
        window.display();
    }

    // Say goodbye to the user!
    // (only if everything went ok)
    std::cout << "\nHave a nice day!"
              << std::endl;
    return 0;
}

std::queue<nq::Level> load(Argument type, char** argv) {
    std::queue<nq::Level> level_queue;
    if (type == Argument::LEVEL) {
        std::string level_file {argv[2]};
        nq::Level level {share + level_file};
        level_queue.push(level);
    } else if (type == Argument::PACK) {
        std::string pack_identifier {argv[2]};
        nq::Pack pack {share + pack_identifier};
        for (const std::string& name : pack.get_levels()) {
            std::string directory {pack.get_directory()};
            nq::Level level {share + directory + name};
            level_queue.push(level);
        }
    }

    return level_queue;
}

Argument pargs(int argc, char** argv) {
    if (argc == 1) {
        return Argument::NONE;
    } else if (argc == 2) {
        if (!std::strcmp(argv[1], "help")) return Argument::HELP_NEEDED;
        // else return Argument::ROOT; // Might be a bit too confusing?
        else return Argument::HELP_NEEDED; // Replace with root later.
    } else if (argc > 2) {
        if (argc != 3) return Argument::HELP_NEEDED;
        else if (!std::strcmp(argv[1], "level")) return Argument::LEVEL;
        else if (!std::strcmp(argv[1], "pack")) return Argument::PACK;
    }

    // How did we get over here?!
    return Argument::HELP_NEEDED;
}

void help(const char* path) {
    std::cout << "Not Quite Sokoban " << version << std::endl;
    std::cout << "usage: " << path << " <argument>" << std::endl;
    std::cout << "<argument> ::= help" << " 'shows this very exciting, calming and useless message'" << std::endl;
    std::cout << "<argument> ::= pack <pack-file>" << " 'loads <pack-file> within the current relative path'" << std::endl;
    std::cout << "<argument> ::= level <level-file>" <<  " 'loads <level-file> within the current relative path'" << std::endl;
    std::cout << "<argument> ::= <root-file>" << " 'sets the root nq information file to <root-file>'" << std::endl;
}
