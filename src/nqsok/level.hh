#ifndef NQSOK_LEVEL_HH
#define NQSOK_LEVEL_HH

#include <vector>
#include <string>
#include <stdexcept>
#include <json/json.h>
#include "color.hh"

namespace nq {
    class Level_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Level final {
    public:
        struct Palette {
            Color<unsigned char> empty {0, 0, 0};
            Color<unsigned char> player {0, 0, 255};
            Color<unsigned char> moveable {255, 0, 0};
            Color<unsigned char> objective {0, 255, 0};
            Color<unsigned char> background {30, 30, 30};
        };

        Level(const std::string&);
        const std::string& get_title() const { return title; }
        const std::string& get_author() const { return author; }
        unsigned get_width() const { return width; }
        unsigned get_height() const { return height; }
        unsigned get_depth() const { return depth; }
        const Palette& get_palette() const { return palette; }
        const std::string& get_directory() const { return directory; }
        const std::string& get_level_path() const { return level_path; }
        const std::vector<std::string>& get_layers() const { return layers; }

        using Layer = std::vector<Color<unsigned char>>;
        using Data = std::vector<Layer>;
        Data data(const std::string&) const;

    private:
        Palette palette;
        std::string title;
        std::string author;
        unsigned width, height, depth;
        std::string directory;
        std::string level_path;
        std::vector<std::string> layers;
        Color<unsigned char> get_color(const Json::Value&,
                                       const std::string&) const;
    };
}

#endif
