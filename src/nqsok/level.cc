#include "level.hh"

#include <fstream>
#include <iostream>
#include "image.hh"

constexpr float nq::Level::VOXEL_SIZE;
nq::Level::Level(const std::string& path)
                : level_path {path} {
    std::cout << "\nLevel (parsing)..." << std::endl;
    std::cout << "Level is '" << path << "'" << std::endl;
    path_parent = level_path.substr(0, level_path.find_last_of("/") - 1);

    std::ifstream file {path};
    if (!file) throw Level_error{"Level error (#1) no such file!"};
    std::cout << "File exists, parsing JSON..." << std::endl;

    Json::Value root; file >> root;
    title = root.get("title", "Untitled").asString();
    std::cout << "Title is '" << title << "'\n";
    author = root.get("author", "Anonymous").asString();
    std::cout << "Author is '" << author << std::endl;

    Json::Value size_value {root["width"]};
    if (!size_value) throw Level_error{"Level error (#2) width needs to be given!"};
    width = size_value.asUInt();
    size_value = root["height"];
    if (!size_value) throw Level_error{"Level error (#3) height needs to be given!"};
    height = size_value.asUInt();
    size_value = root["depth"];
    if (!size_value) throw Level_error{"Level error (#4) depth needs to be given!"};
    depth = size_value.asUInt();
    std::cout << "Size of ("
              << width << ", "
              << height << ", "
              << depth << ")\n";

    Json::Value directory_value = root["directory"];
    if (!directory_value) throw Level_error{"Level error (#5) no directory specified!"};
    directory = directory_value.asString();
    std::cout << "Directory is '" << directory << "'" << std::endl;

    Json::Value palette_value = root["palette"];
    if (!!palette_value) {
        palette.empty = get_color(palette_value, "empty");
        palette.player = get_color(palette_value, "player");
        palette.moveable = get_color(palette_value, "moveable");
        palette.objective = get_color(palette_value, "objective");
        palette.background = get_color(palette_value, "background");
        std::cout << "Palette specified" << std::endl;
    }

    Json::Value layers_value = root["layers"];
    if (!layers_value) throw Level_error{"Level error (#6) at least one layer needs to exist!"};
    layers.reserve(layers_value.size());
    for (std::size_t i {0}; i < layers_value.size(); ++i) {
        layers.push_back(layers_value[static_cast<int>(i)].asString());
    }

    std::cout << "Found " << layers.size() << " layer(s)" << std::endl;
    std::cout << "...done (Level)" << std::endl;
}

nq::Color<unsigned char> nq::Level::get_color(const Json::Value& node,
                                              const std::string& name) const {
    Color<unsigned char> result;
    result.red = node[name][0].asUInt();
    result.green = node[name][1].asUInt();
    result.blue = node[name][2].asUInt();
    return result;
}

nq::Level::Data nq::Level::data(const std::string& ldir) const {
    Data level_data;
    level_data.reserve(height);
    for (unsigned h {0}; h < height; ++h) {
        unsigned size {width*depth};
        nq::Image layer_image {ldir + directory + layers[h]};
        Layer layer_data; layer_data.reserve(size);
        for (unsigned i {0}; i < size; ++i) {
            layer_data.push_back({layer_image[0 + i*3],
                                  layer_image[1 + i*3],
                                  layer_image[2 + i*3]});
        }

        level_data.push_back(layer_data);
    }

    return level_data;
}
