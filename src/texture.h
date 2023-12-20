#ifndef TEXTURE_H
#define TEXTURE_H

#include <cinttypes>
#include <cstring>
// #include <SFML/Graphics.hpp>

namespace plugin {
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        explicit Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255);

        explicit Color();
    };

    struct Texture {
        uint64_t height;
        uint64_t width;

        Color *pixels;

        explicit Texture(uint64_t w, uint64_t h, const Color* pix = nullptr);

        virtual ~Texture() {
            delete[] pixels;
        };
    };
}

#endif