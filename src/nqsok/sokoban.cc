#include "sokoban.hh"
#include "color.hh"

nq::Sokoban::Sokoban(const Level& level, const Level::Data& data)
                    : level {level}, level_data {data} {
    // Complex loop :S. Basically, find the color of the block located
    // at {x, y, z} in the level data. Retrieve vector if interesting.
    for (unsigned y {0}; y < level.get_height(); ++y) {
        for (unsigned z {0}; z < level.get_depth(); ++z) {
            for (unsigned x {0}; x < level.get_width(); ++x) {
                unsigned i {z * level.get_width() + x};
                nq::Color<unsigned char> color {level_data[y][i]};
                if (color == level.get_palette().player) player_position = {x, y, z};
                else if (color == level.get_palette().moveable) moveable_positions.push_back({x, y, z});
                else if (color == level.get_palette().objective) objective_positions.push_back({x, y, z});
            }
        }
    }
}

bool nq::Sokoban::success() const {
    return false;
}

bool nq::Sokoban::undo() {
    return false;
}

bool nq::Sokoban::step(const Action&) {
    return false;
}

void nq::Sokoban::reset(const Level::Data&) {
}

