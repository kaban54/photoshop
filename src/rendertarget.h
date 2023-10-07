#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <SFML/Graphics.hpp>
#include "region.h"
#include "vec.h"
#include "color.h"

class RenderTarget {
    sf::RenderTexture screen;

    public:

    RenderTarget (unsigned int w, unsigned int h);

    void Display (sf::RenderWindow& window) const;


    void DrawRect (const Rect& rect, const Color& col, const RegionSet& regset);

    void DrawLine (const Vec& p1, const Vec& p2, const Color& col, const RegionSet& regset);

    void SetPixel (const Vec& point, const Color& col, const RegionSet& regset);

};

#endif