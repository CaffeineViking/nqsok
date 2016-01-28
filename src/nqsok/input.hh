#ifndef NQSOK_INPUT_HH
#define NQSOK_INPUT_HH

#include <stdexcept>
#include "window.hh"

namespace nq {
    class Input_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Input final {
    public:
        struct Position {
            double x, y;
        };

        static bool key_pressed(const Window&, int);
        static bool key_released(const Window&, int);
        static Position mouse_position(const Window&);
    };
}

#endif
