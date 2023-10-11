#include "rendertarget.h"

RenderTarget::RenderTarget (unsigned int w, unsigned int h):
    width (w),
    height (h),
    screen ()
    {
        screen.create (w, h);
    }

void RenderTarget::Display (sf::RenderWindow& window) const {
    sf::Sprite sprite;
    sprite.setTexture (screen.getTexture());
    sprite.setPosition (0, 0);
    window.draw (sprite);
}

void RenderTarget::ClearScreen (const sf::Color& color) {
    screen.clear (color);
    screen.display();
}

void RenderTarget::DrawRect (const Rect& rect, const Color& col, const RegionSet& regset) {
    sf::RectangleShape rectshape (sf::Vector2f (rect.GetWidth(), rect.GetHeight()));
    rectshape.setFillColor (sf::Color (col.r, col.g, col.b));
    rectshape.setPosition (rect.vert1.x, rect.vert1.y);
    screen.draw(rectshape);
    screen.display();
}

void RenderTarget::DrawLine (const Vec& p1, const Vec& p2, const Color& col, const RegionSet& regset) {    
    sf::Color color (col.r, col.g, col.b, col.a);
    sf::Vertex line[] = {
        sf::Vertex (sf::Vector2f (p1.x, p1.y), color),
        sf::Vertex (sf::Vector2f (p2.x, p2.y), color)
    };
    screen.draw (line, 2, sf::Lines);
    screen.display();
}

void RenderTarget::SetPixel (const Vec& point, const Color& col, const RegionSet& regset) {
    sf::Color color (col.r, col.g, col.b, col.a);
    sf::Vertex pixel[] = {
        sf::Vertex (sf::Vector2f(point.x, point.y), color)
    };
    screen.draw (pixel, 1, sf::Points);
    screen.display();
}