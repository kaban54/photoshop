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

void RenderTarget::DrawRect (const Rect& rect, const Color& col, const RegionSet& to_draw) {
    RegionSet tmp, rects;
    tmp.AddRegion (rect);

    IntersectRegsets (tmp, to_draw, rects);

    sf::RectangleShape rectshape;
    rectshape.setFillColor (sf::Color (col.r, col.g, col.b));
    
    // rectshape.setOutlineThickness (2);
    // rectshape.setOutlineColor (sf::Color::Red);

    ListNode<Rect>* end_of_list = rects.regions.EndOfList();
    ListNode<Rect>* node = rects.regions.GetHead();

    while (node != end_of_list) {
        Rect region = node -> val;

        rectshape.setSize (sf::Vector2f(region.GetWidth(), region.GetHeight()));
        rectshape.setPosition (region.vert1.x, region.vert1.y);
        screen.draw (rectshape);
        screen.display();

        node = node -> next;
    }
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

void RenderTarget::DrawTexture (const Texture& texture, const Vec& pos, const Vec& size, const RegionSet& regset) {
    sf::Sprite sprite;
    sprite.setTexture (*(texture.sftexture));
    sprite.setPosition (pos.x, pos.y);
    sprite.setScale (size.x / texture.sftexture -> getSize().x, size.y / texture.sftexture -> getSize().y);

    screen.draw (sprite);
    screen.display();
/*
    ListNode<Rect>* end_of_list = regset.regions.EndOfList();
    ListNode<Rect>* node = regset.regions.GetHead();

    while (node != end_of_list) {
        Rect region = node -> val;
        sprite.setTextureRect (sf::IntRect(region.vert1.x - pos.x, region.vert1.y - pos.y,
                                           region.GetWidth(), region.GetHeight()));
        screen.draw (sprite);
        screen.display();

        node = node -> next;
    }*/
}