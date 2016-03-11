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
            Color<char> empty {(char)0, (char)0, (char)0};
            Color<char> player {(char)0, (char)0, (char)255};
            Color<char> moveable {(char)255, (char)0, (char)0};
            Color<char> objective {(char)0, (char)255, (char)0};
        };

        Level(const std::string&);
        const std::string& get_title() const { return title; }
        const std::string& get_author() const { return author; }
        unsigned get_width() const { return width; }
        unsigned get_height() const { return height; }
        unsigned get_depth() const { return depth; }
        const Palette& get_palette() const { return palette; }
        const std::string& get_directory() const { return directory; }
        const std::vector<std::string>& get_layers() const { return layers; }

    private:
        Palette palette;
        std::string title;
        std::string author;
        unsigned width, height, depth;
        std::string directory;
        std::vector<std::string> layers;
        Color<char> get_color(const Json::Value&,
                              const std::string&) const;
    };
}

#endif
