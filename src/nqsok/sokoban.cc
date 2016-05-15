#include "sokoban.hh"
#include <iostream>
#include "color.hh"

bool nq::Sokoban::success() const {
    for (const Position& objective_position : objective_positions)
        if (type(objective_position) != Block::MOVEABLE) return false;
    return true;
}

bool nq::Sokoban::undo() {
    if (past.size() == 0) return false;
    std::size_t size {moveable_positions.size() + 1};
    if (past.size() <= size) { reset(); return true; }
    for (std::size_t i {0}; i < size; ++i) past.pop();
    player_position = past.top(); past.pop();
    for (Position& moveable_position : moveable_positions) {
        moveable_position = past.top();
        past.pop();
    }

    store_past_positions();
    actions.pop();
    return true;
}

bool nq::Sokoban::step(const Action& action) {
    if (!collides(player_position, action)) {
        Position future_position {future(player_position, action)};
        Position ground_position {bottom(future_position)};
        if (player_position == ground_position) return false;
        else if (ground_position.y == 0) return false;
        player_position = ground_position;
        store_past_positions();
        actions.push(action);
        return true;
    } else {
        Position future_position {future(player_position, action)};
        Block future_block_type {type(future_position)};
        if (future_block_type == Block::IMMOVABLE) {
            Position position_above {top(player_position)};
            Position position_behind {reverse(player_position, action)};
            if (position_above.y - player_position.y > 1) return false;
            if (collides(position_behind)) return false; // Stuck, eh?
            Position roof_position {top(future_position)};
            if (roof_position.y - future_position.y > 1 ||
                roof_position.y == level.get_height()) return false;
            if (player_position == roof_position) return false;
            player_position = roof_position;
            store_past_positions();
            actions.push(action);
            return true;
        } else if (future_block_type == Block::MOVEABLE) {
            Position affected_position {future(future_position, action)};
            if (collides(affected_position)) { // To infinity, and beyond!
                Position position_behind {reverse(player_position, action)};
                if (collides(position_behind)) return false; // Stuck, eh?
                Position roof_position {top(future_position)};
                if (roof_position.y - future_position.y > 1 ||
                    roof_position.y == level.get_height()) return false;
                if (player_position == roof_position) return false;
                player_position = roof_position;
                store_past_positions();
                actions.push(action);
                return true;
            }

            Position ground_position {bottom(affected_position)};
            if (player_position == future_position) return false;
            else if (ground_position.y == 0) return false;
            moveable(future_position, ground_position);
            player_position = future_position;
            store_past_positions();
            actions.push(action);
            return true;
        }
    }

    // What happened?
    return false;
}

void nq::Sokoban::check() {
    for (const Position& p : moveable_positions) {
        if (p.y >= level.get_height() ||
            p.y == 0) throw Sokoban_error{"Sokoban error (#1) out of bounds."};
        if (type({p.x, p.y - 1, p.z}) != Block::IMMOVABLE) {
            throw Sokoban_error{"Sokoban error (#2) solid not under moveable!"};
        }
    }

    for (const Position& p : objective_positions) {
        if (p.y >= level.get_height()
            || p.y == 0) throw Sokoban_error{"Sokoban error (#3) out of bounds."};
        if (type({p.x, p.y - 1, p.z}) != Block::IMMOVABLE) {
            throw Sokoban_error{"Sokoban error (#4) solid not under objective!"};
        }
    }

    if (player_position.y >= level.get_height()
        || player_position.y == 0) throw Sokoban_error{"Sokoban error (#5) out of bounds."};
    if (type({player_position.x, player_position.y - 1, player_position.z}) != Block::IMMOVABLE) {
        throw Sokoban_error{"Sokoban error (#6) solid not under player, player is very kill!"};
    }

    if (moveable_positions.size() < objective_positions.size())
        throw Sokoban_error{"Sokoban error (#8) impossible level"};
}

void nq::Sokoban::reset() {
    unsigned int players {0};
    moveable_positions.clear();
    objective_positions.clear();
    actions = std::stack<Action>{};
    past = std::stack<Position>{};
    for (unsigned y {0}; y < level.get_height(); ++y) {
        for (unsigned z {0}; z < level.get_depth(); ++z) {
            for (unsigned x {0}; x < level.get_width(); ++x) {
                unsigned i {z * level.get_width() + x};
                nq::Color<unsigned char> color {level_data[y][i]};
                if (color == level.get_palette().player)
                    { player_position = {x, y, z}; ++players; }
                else if (color == level.get_palette().moveable)
                    moveable_positions.push_back({x, y, z});
                else if (color == level.get_palette().objective)
                    objective_positions.push_back({x, y, z});
            }
        }
    }

    // Do a quick check to see that there is only a single player entity, otherwise weird states can happen.
    if (players != 1) throw Sokoban_error{"Sokoban error (#7) there exists more than one player voxel, ok!"};
}

bool nq::Sokoban::objective(const Position& position) const {
    for (const Position& objective_position : objective_positions)
        if (objective_position == position) return true;
    return false;
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
    for (long int height = position.y; height < level.get_height(); ++height) {
        Position empty_position = {position.x, (unsigned)height, position.z};
        if (!collides(empty_position)) return empty_position; // Top kek yes.
    }

    // At the top of the level, might want to skip this.
    return {position.x, level.get_height(), position.z};
}

nq::Sokoban::Position nq::Sokoban::bottom(const Position& position) const {
    for (long int height = position.y; height >= 0; --height) {
        Position collidable_position = {position.x, (unsigned)height, position.z};
        Position ground_position = {position.x, (unsigned)(height + 1), position.z};
        if (collides(collidable_position)) return ground_position;
    }

    // Will fall to the void, not ok...
    return {position.x, 0, position.z};
}

nq::Sokoban::Position nq::Sokoban::future(Position position, const Action& action) const {
    if (action == Action::FORWARD && position.z < (level.get_depth() - 1)) ++position.z;
    if (action == Action::RIGHT && position.x < (level.get_width() - 1)) ++position.x;
    if (action == Action::BACKWARD && position.z >= 1) --position.z;
    if (action == Action::LEFT && position.x >= 1) --position.x;
    return position;
}

nq::Sokoban::Position nq::Sokoban::reverse(Position of, const Action& action) const {
    Position position = of;
    if (action == Action::FORWARD) return future(of, Action::BACKWARD);
    if (action == Action::RIGHT) return future(of, Action::LEFT);
    if (action == Action::BACKWARD) return future(of, Action::FORWARD);
    if (action == Action::LEFT) return future(of, Action::RIGHT);
    return position;
}

// Helper function to set the position of a moveable block, needs search...
void nq::Sokoban::moveable(const Position& moveable, const Position& position) {
    for (Position& moveable_position : moveable_positions) {
        if (moveable_position == moveable) {
            moveable_position = position;
            return;
        }
    }
}

bool nq::Sokoban::collides(const Position& of, const Action& action) const {
    Position future_position {future(of, action)};
    return collides(future_position);
}

bool nq::Sokoban::collides(const Position& position) const {
    Block block_type {type(position)};
    if (block_type == Block::IMMOVABLE
        || block_type == Block::MOVEABLE
        || block_type == Block::PLAYER) return true;
    else return false;
}

// Stack can grow quite big, might want to
// find a better solution later? Assuming,
// 128 bytes per move & 128 moves ~1KiB?!?
void nq::Sokoban::store_past_positions() {
    for (const Position& moveable_position : moveable_positions)
        past.push(moveable_position);
    past.push(player_position);
}
