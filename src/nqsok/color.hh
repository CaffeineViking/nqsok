#ifndef NQSOK_COLOR_HH
#define NQSOK_COLOR_HH

namespace nq {
    template<typename T>
    struct Color final {};

    template<>
    struct Color<float> final {
        float red,
              green,
              blue;
    };

    template<>
    struct Color<unsigned char> final {
        operator Color<float>() const {
            return {red / 255.0f,
                    green / 255.0f,
                    blue / 255.0f};
        }

        bool operator!=(const Color<unsigned char>& other) { return !(*this == other); }
        bool operator==(const Color<unsigned char>& other) {
            return (red == other.red
                    && green == other.green
                    && blue == other.blue);
        }

        unsigned char red,
                      green,
                      blue;
    };
}

#endif
