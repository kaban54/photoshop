#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <SFML/Graphics.hpp>
#include "region.h"
#include "vec.h"
#include "color.h"


struct Texture {
    sf::Texture* sftexture;
};

struct Text {
    sf::Text* sftxt;
};

class RenderTarget {
    unsigned int width;
    unsigned int height;
    sf::RenderTexture screen;

    public:

    RenderTarget (unsigned int w, unsigned int h);

    void Display (sf::RenderWindow& window) const;

    void ClearScreen (const sf::Color& color);

    void DrawRect (const Rect& rect, const Color& col, const RegionSet& regset);

    void DrawLine (const Vec& p1, const Vec& p2, const Color& col, const RegionSet& regset);

    void SetPixel (const Vec& point, const Color& col, const RegionSet& regset);

    void DrawTexture (const Texture& texture, const Vec& pos, const Vec& size, const RegionSet& regset);
};

#endif