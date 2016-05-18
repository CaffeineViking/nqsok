#ifndef NQSOK_COLOR_HH
#define NQSOK_COLOR_HH

#include <glm/glm.hpp>

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

        operator glm::vec3() const {
            return {red / 255.0f,
                    green / 255.0f,
                    blue / 255.0f};
        }

        Color<unsigned char> operator+(const Color<unsigned char>& other) {
            return {static_cast<unsigned char>((*this).red + other.red),
                    static_cast<unsigned char>((*this).green + other.green),
                    static_cast<unsigned char>((*this).blue + other.blue)};
        }

        Color<unsigned char> operator-(const Color<unsigned char>& other) {
            return {static_cast<unsigned char>((*this).red - other.red),
                    static_cast<unsigned char>((*this).green - other.green),
                    static_cast<unsigned char>((*this).blue - other.blue)};
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
