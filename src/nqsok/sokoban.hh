#ifndef NQSOK_SOKOBAN_HH
#define NQSOK_SOKOBAN_HH

#include "level.hh"
#include <glm/glm.hpp>
#include <vector>
#include <stack>

namespace nq {
    class Sokoban final {
    public:
        struct Position { unsigned x, y, z; };
        using Positions = std::vector<Position>;
        enum class Action {
            FORWARD, RIGHT,
            BACKWARD, LEFT,
            CLIMB, DESCEND
        };

        Sokoban(const Level& level, const Level::Data& data);
        bool success() const; // Check if: winning! Winning! WINNING!
        bool undo(); // Latest action on stack is undone. Fails if empty.
        bool step(const Action&); // Does step, fails if not valid with rule.
        void reset(const Level::Data&); // Copies state of initial level.

        const Position& get_player() const { return player_position; }
        const Positions& get_moveables() const { return moveable_positions; }
        const Positions& get_objectives() const { return objective_positions; }

    private:
        const Level& level;
        const Level::Data& level_data;
        std::stack<Action> actions;
        Position player_position;
        Positions moveable_positions;
        Positions objective_positions;
    };
}

#endif
