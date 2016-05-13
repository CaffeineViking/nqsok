#ifndef NQSOK_SOKOBAN_HH
#define NQSOK_SOKOBAN_HH

#include "level.hh"
#include <stdexcept>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stack>

namespace nq {
    class Sokoban_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Sokoban final {
    public:
        struct Position {
            bool operator==(const Position& p) const { return x == p.x && y == p.y && z == p.z; }
            bool operator!=(const Position& other) const { return !(*this == other); }
            operator glm::vec3() const {
                glm::vec3 result {static_cast<float>(x),
                                  static_cast<float>(y),
                                  static_cast<float>(z)};
                return result;
            }

            unsigned x, y, z;
        };

        using Positions = std::vector<Position>;
        enum class Action {
            FORWARD, RIGHT,
            BACKWARD, LEFT
        };

        Sokoban(const Level& level, const Level::Data& data)
               : level {level}, level_data {data} { reset(); check(); }

        bool success() const; // Check if: winning! Winning! WINNING!
        bool undo(); // Latest action on stack is undone. Fails if empty.
        bool step(const Action&); // Does step, fails if not valid with rule.
        void reset(); // Copies state of initial level, which is level_data.
        void check(); // Makes sure this level is valid before starting it.

        std::size_t get_moves() const { return actions.size(); }
        const Position& get_player() const { return player_position; }
        const Positions& get_moveables() const { return moveable_positions; }
        const Positions& get_objectives() const { return objective_positions; }

    private:
        enum class Block {
            EMPTY, IMMOVABLE,
            PLAYER, MOVEABLE,
            OBJECTIVE
        };

        // Helper functions for step().
        Block type(const Position&) const;
        Position top(const Position&) const;
        bool collides(const Position&) const;
        Position bottom(const Position&) const;
        // Predict future of a certain block with an action.
        bool collides(const Position&, const Action&) const;
        void moveable(const Position&, const Position&);
        Position reverse(Position, const Action&) const;
        Position future(Position, const Action&) const;

        const Level& level;
        const Level::Data& level_data;
        std::stack<Action> actions;
        Position player_position;
        Positions moveable_positions;
        Positions objective_positions;
    };

}

#endif
