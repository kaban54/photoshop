#include "texture.h"

namespace plugin {
    Color::Color (uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_):
        r (r_),
        g (g_),
        b (b_),
        a (a_) {}

    Color::Color():
        r (0),
        g (0),
        b (0),
        a (0) {}

    sf::Color Color::GetSfColor() const {
        return sf::Color(r, g, b, a);
    }

    Texture::Texture(uint64_t w, uint64_t h, const Color* pix):
        width (w),
        height (h),
        pixels (new Color[w * h])
        {
            if (pix != nullptr) {
                memcpy (pixels, pix, w * h * sizeof(pixels[0]));
            }
        }
}