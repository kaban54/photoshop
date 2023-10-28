#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color (uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255);
};

#endif