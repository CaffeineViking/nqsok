#include "sokoban.hh"
#include <iostream>
#include "color.hh"

bool nq::Sokoban::success() const {
    for (const Position& objective_position : objective_positions)
        if (type(objective_position) != Block::MOVEABLE) return false;
    return true;
}

bool nq::Sokoban::undo() {
    // TODO:  complicated?
    return false;
}

bool nq::Sokoban::step(const Action& action) {
    std::cout << "Action: "
              << saction(action)
              << std::endl;

    // Step the rules of the game in regards to player.
    Position position {future(player_position, action)};
    Block block_types {type(position)};
    if (block_types == Block::EMPTY
        || block_types == Block::OBJECTIVE) {
        Position ground = bottom(position);
        if (ground.y == 0) return false;
        else {
            // Players can fall down.
            player_position = ground;
            actions.push(action);

            if (position.y - player_position.y != 0) {
                std::cout << "Event: player fell down to "
                          << sposition(player_position)
                          << std::endl;
            } else {
                std::cout << "Event: player moved to "
                          << sposition(player_position)
                          << std::endl;
            }

            return true;
        }
    } else if (block_types == Block::IMMOVABLE) {
        Position roof = top(position);
        if (roof.y - position.y != 1) return false;
        else {
            Position back {reverse(player_position, action)};
            Block back_block {type(back)};
            if (back_block == Block::IMMOVABLE
                || back_block == Block::EMPTY) return false;

            // Player can climb up.
            player_position = roof;
            actions.push(action);

            std::cout << "Event: player climbed up to "
                      << sposition(player_position)
                      << std::endl;

            return true;
        }
    } else if (block_types == Block::MOVEABLE) {
        Position moveable {future(position, action)};
        Block future_block {type(moveable)};
        if (future_block == Block::EMPTY
            || future_block == Block::OBJECTIVE) {
            Position ground = bottom(moveable);
            if (ground.y != 0) {
                // Moveable should fall down now.
                moveable_set(position, moveable);
                player_position = position;
                actions.push(action);

                std::cout << "Event: player moved to "
                          << sposition(player_position)
                          << std::endl;
                if (moveable.y - position.y == 0) {
                    std::cout << "Event: player pushed moveable to "
                            << sposition(moveable)
                            << std::endl;
                } else {
                    std::cout << "Event: player pushed moveable down to "
                            << sposition(moveable)
                            << std::endl;
                }

                return true;
            }
        } else if (future_block == Block::IMMOVABLE
                   || future_block == Block::MOVEABLE
                   || future_block == Block::EMPTY) {
            Position roof = top(position);
            if (roof.y - position.y != 1) return false;
            else {
                // Player can climb up.
                player_position = roof;
                actions.push(action);

                std::cout << "Event: player climbed moveable to "
                          << sposition(player_position)
                          << std::endl;

                return true;
            }
        }
    }

    return false;
}

void nq::Sokoban::check() {
    for (const Position& p : moveable_positions) {
        if (p.y > level.get_height() ||
            p.y == 0) throw Sokoban_error{"Sokoban error (#1) out of bounds."};
        if (type({p.x, p.y - 1, p.z}) != Block::IMMOVABLE) {
            throw Sokoban_error{"Sokoban error (#2) solid not under moveable!"};
        }
    }

    for (const Position& p : objective_positions) {
        if (p.y > level.get_height()
            || p.y == 0) throw Sokoban_error{"Sokoban error (#3) out of bounds."};
        if (type({p.x, p.y - 1, p.z}) != Block::IMMOVABLE) {
            throw Sokoban_error{"Sokoban error (#4) solid not under objective!"};
        }
    }

    if (player_position.y > level.get_height()
        || player_position.y == 0) throw Sokoban_error{"Sokoban error (#5) out of bounds."};
    if (type({player_position.x, player_position.y - 1, player_position.z}) != Block::IMMOVABLE) {
        throw Sokoban_error{"Sokoban error (#6) solid not under player, player is very kill!"};
    }
}

void nq::Sokoban::reset() {
    moveable_positions.clear();
    objective_positions.clear();
    actions = std::stack<Action>{};
    // Find out color {r, g, b} -> type -> {x, y, z}.
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

nq::Sokoban::Block nq::Sokoban::type(const Position& position) const {
    for (const Position& moveable_position : moveable_positions)
        if (moveable_position == position) return Block::MOVEABLE;
    for (const Position& objective_position : objective_positions)
        if (objective_position == position) return Block::OBJECTIVE;
    if (position == player_position) return Block::PLAYER;

    unsigned i {position.z * level.get_width() + position.x};
    if (position.y >= level.get_height()) return Block::EMPTY;
    nq::Color<unsigned char> color {level_data[position.y][i]};
    const nq::Level::Palette& palette {level.get_palette()};
    if (color == palette.empty
        || color == palette.player
        || color == palette.moveable
        || color == palette.objective) return Block::EMPTY;
    else return Block::IMMOVABLE; // Aww yea, dem colors!!!
}

nq::Sokoban::Position nq::Sokoban::top(const Position& position) const {
    // Proceed up the rabbit hole, return the roof block.
    for (unsigned layer {position.y}; layer != level.get_height(); ++layer) {
        if (type({position.x, layer, position.z}) == Block::EMPTY
            || type({position.x, layer, position.z}) == Block::OBJECTIVE)
            return Position{position.x, layer, position.z};
    }

    // At the top of the level, being in this state isn't good..
    return Position{position.x, level.get_height(), position.z};
}

nq::Sokoban::Position nq::Sokoban::bottom(const Position& position) const {
    // Proceed down the rabbit hole, return the ground block.
    for (unsigned layer {position.y}; (layer + 1) != 0; --layer) {
        if (type({position.x, layer, position.z}) != Block::EMPTY) {
            if (type({position.x, layer, position.z}) == Block::OBJECTIVE)
                return Position{position.x, layer, position.z};
            return Position{position.x, layer + 1, position.z};
        }
    }

    // At bottom, can't fall down OK? Is kill!!
    return Position{position.x, 0, position.z};
}

nq::Sokoban::Position nq::Sokoban::future(const Position& of, const Action& action) const {
    Position position = of;
    if (action == Action::FORWARD && position.z < (level.get_depth() - 1)) ++position.z;
    if (action == Action::RIGHT && position.x < (level.get_width() - 1)) ++position.x;
    if (action == Action::BACKWARD && position.z >= 1) --position.z;
    if (action == Action::LEFT && position.z >= 1) --position.x;
    return position;
}

nq::Sokoban::Position nq::Sokoban::reverse(const Position& of, const Action& action) const {
    Position position = of;
    if (action == Action::FORWARD) return future(of, Action::BACKWARD);
    if (action == Action::RIGHT) return future(of, Action::LEFT);
    if (action == Action::BACKWARD) return future(of, Action::FORWARD);
    if (action == Action::LEFT) return future(of, Action::RIGHT);
    return position;
}

void nq::Sokoban::moveable_set(const Position& moveable, const Position& position) {
    for (Position& moveable_position : moveable_positions) {
        if (moveable_position == moveable) {
            moveable_position = position;
            return;
        }
    }
}
