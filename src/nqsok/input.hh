#ifndef NQSOK_INPUT_HH
#define NQSOK_INPUT_HH

#include <stdexcept>
#include "window.hh"

namespace nq {
    class Input_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Input final {
    public:
        static bool state(const Window&, const std::string&);
        static double value(const Window&, const std::string&);

        static void grab_cursor(const Window&);
        static void hide_cursor(const Window&);
        static void normal_cursor(const Window&);

    private:
        struct Position {
            double x, y;
        };

        static bool key_pressed(const Window&, int);
        static bool key_released(const Window&, int);

        static Position mouse_position(const Window&);
        static bool mouse_pressed(const Window&, int);
        static bool mouse_released(const Window&, int);
    };
}

#endif
