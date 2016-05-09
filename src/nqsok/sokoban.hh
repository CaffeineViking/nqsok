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
        Position bottom(const Position&) const;
        // Predict future of a certain block with an action.
        Position future(const Position&, const Action&) const;
        Position reverse(const Position&, const Action&) const;
        void moveable_set(const Position&, const Position&);

        std::string saction(const Action& action) const {
            if (action == Action::FORWARD) return "forward";
            else if (action == Action::RIGHT) return "right";
            else if (action == Action::BACKWARD) return "backward";
            else if (action == Action::LEFT) return "left";
            return "what have you done?";
        }

        std::string sposition(const Position& position) const {
            std::string result {"("};
            result += std::to_string(position.x) + ", ";
            result += std::to_string(position.y) + ", ";
            result += std::to_string(position.z) + ")";
            return result;
        }

        const Level& level;
        const Level::Data& level_data;
        std::stack<Action> actions;
        Position player_position;
        Positions moveable_positions;
        Positions objective_positions;
    };

}

#endif
