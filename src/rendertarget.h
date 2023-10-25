#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <SFML/Graphics.hpp>
#include "region.h"
#include "vec.h"
#include "color.h"


struct Texture {
    sf::Texture* sftexture;

    explicit Texture () {}

    explicit Texture (sf::Texture* sftexture_): sftexture (sftexture_) {}
};

struct Text {
    sf::Text* sftxt;

    explicit Text () {}

    explicit Text (sf::Text* sftxt_): sftxt (sftxt_) {} 
};

class RenderTarget {
    unsigned int width;
    unsigned int height;
    sf::RenderTexture screen;

    public:

    RenderTarget (unsigned int w, unsigned int h);

    void Display (sf::RenderWindow& window) const;

    void ClearScreen (const Color& col);

    void DrawRect (const Rect& rect, const Color& col, RegionSet* to_draw = nullptr);

    void DrawLine (const Vec& p1, const Vec& p2, const Color& col, RegionSet* to_draw = nullptr);

    void SetPixel (const Vec& point, const Color& col, RegionSet* to_draw = nullptr);

    void DrawTexture (const Texture& texture, const Vec& pos, const Vec& size, RegionSet* to_draw = nullptr);

    void DrawText (const Text& txt, const Vec& pos, const Color& col, RegionSet* to_draw = nullptr);

    void DrawRenderTarget (const RenderTarget& rt, const Vec& pos, RegionSet* to_draw = nullptr);

    void DrawCircle (const Vec& pos, double radius, const Color& col, RegionSet* to_draw = nullptr);

    void DrawEllipse (const Rect& rect, const Color& col, RegionSet* to_draw = nullptr);
};

#endif