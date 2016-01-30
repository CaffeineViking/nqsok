#ifndef NQSOK_COLOR_HH
#define NQSOK_COLOR_HH

namespace nq {
    template<typename T>
    struct Color final {};

    template<>
    struct Color<char> final {
        char red,
             green,
             blue;
    };

    template<>
    struct Color<float> final {
        float red,
              green,
              blue;
    };
}

#endif
