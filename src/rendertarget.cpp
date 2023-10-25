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

void RenderTarget::ClearScreen (const Color& col) {
    screen.clear (sf::Color(col.r, col.g, col.b, col.a));
    screen.display();
}

void RenderTarget::DrawRect (const Rect& rect, const Color& col, RegionSet* to_draw) {
    sf::RectangleShape rectshape;
    rectshape.setFillColor (sf::Color (col.r, col.g, col.b));

    if (to_draw == nullptr) {
        rectshape.setSize (sf::Vector2f(rect.GetWidth(), rect.GetHeight()));
        rectshape.setPosition (rect.vert1.x, rect.vert1.y);
        screen.draw (rectshape);
        screen.display();
    }
    else {
        RegionSet tmp, rects;
        tmp.AddRegion (rect);
        IntersectRegsets (tmp, *to_draw, rects);    
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
}

void RenderTarget::DrawLine (const Vec& p1, const Vec& p2, const Color& col, RegionSet* regset) {    
    sf::Color color (col.r, col.g, col.b, col.a);
    sf::Vertex line[] = {
        sf::Vertex (sf::Vector2f (p1.x, p1.y), color),
        sf::Vertex (sf::Vector2f (p2.x, p2.y), color)
    };
    screen.draw (line, 2, sf::Lines);
    screen.display();
}

void RenderTarget::SetPixel (const Vec& point, const Color& col, RegionSet* regset) {
    sf::Color color (col.r, col.g, col.b, col.a);
    sf::Vertex pixel[] = {
        sf::Vertex (sf::Vector2f(point.x, point.y), color)
    };
    screen.draw (pixel, 1, sf::Points);
    screen.display();
}

void RenderTarget::DrawTexture (const Texture& texture, const Vec& pos, const Vec& size, RegionSet* regset) {
    sf::Sprite sprite;
    sprite.setTexture (*(texture.sftexture));

    double xscale = size.x / texture.sftexture -> getSize().x;
    double yscale = size.y / texture.sftexture -> getSize().y;
    sprite.setScale (xscale, yscale);

    ListNode<Rect>* end_of_list = regset -> regions.EndOfList();
    ListNode<Rect>* node = regset -> regions.GetHead();

    while (node != end_of_list) {
        Rect region = node -> val;

        double x = (region.vert1.x - pos.x) / xscale;
        double y = (region.vert1.y - pos.y) / yscale;
        double w = region.GetWidth()  / xscale;
        double h = region.GetHeight() / yscale;

        sprite.setPosition (region.vert1.x, region.vert1.y);
        sprite.setTextureRect (sf::IntRect(x, y, w, h));
        screen.draw (sprite);
        screen.display();

        node = node -> next;
    }
}

void RenderTarget::DrawText (const Text& txt, const Vec& pos, const Color& col, RegionSet* regset) {
    sf::Text text = *(txt.sftxt);
    sf::FloatRect bounds = text.getGlobalBounds();
    sf::RenderTexture sfrt;
    sfrt.create(bounds.width, text.getCharacterSize());
    sfrt.clear (sf::Color::Transparent);
    text.setFillColor (sf::Color (col.r, col.g, col.b, col.a));
    text.setPosition (0, 0);
    sfrt.draw (text);
    sfrt.display();

    sf::Sprite sprite;
    sprite.setTexture (sfrt.getTexture());

    ListNode<Rect>* end_of_list = regset -> regions.EndOfList();
    ListNode<Rect>* node = regset -> regions.GetHead();

    while (node != end_of_list) {
        Rect region = node -> val;

        double x = region.vert1.x - pos.x;
        double y = region.vert1.y - pos.y;
        double w = region.GetWidth();
        double h = region.GetHeight();

        sprite.setPosition (region.vert1.x, region.vert1.y);
        sprite.setTextureRect (sf::IntRect(x, y, w, h));
        screen.draw (sprite);
        screen.display();

        node = node -> next;
    }
}

void RenderTarget::DrawRenderTarget (const RenderTarget& rt, const Vec& pos, RegionSet* regset) {
    sf::Sprite sprite;
    sprite.setTexture (rt.screen.getTexture());

    ListNode<Rect>* end_of_list = regset -> regions.EndOfList();
    ListNode<Rect>* node = regset -> regions.GetHead();

    while (node != end_of_list) {
        Rect region = node -> val;

        double x = region.vert1.x - pos.x;
        double y = region.vert1.y - pos.y;
        double w = region.GetWidth();
        double h = region.GetHeight();

        sprite.setPosition (region.vert1.x, region.vert1.y);
        sprite.setTextureRect (sf::IntRect(x, y, w, h));
        screen.draw (sprite);
        screen.display();

        node = node -> next;
    }
}


void RenderTarget::DrawCircle (const Vec& pos, double radius, const Color& col, RegionSet* to_draw) {
    sf::CircleShape circle (radius);
    circle.setPosition (pos.x - radius, pos.y - radius);
    circle.setFillColor (sf::Color(col.r, col.g, col.b, col.a));
    screen.draw (circle);
    screen.display();
}

void RenderTarget::DrawEllipse (const Rect& rect, const Color& col, RegionSet* to_draw) {
    sf::CircleShape ellipse (rect.GetWidth() / 2);
    ellipse.setPosition (rect.vert1.x, rect.vert1.y);
    ellipse.setScale (1, rect.GetHeight() / rect.GetWidth());
    ellipse.setFillColor (sf::Color(col.r, col.g, col.b, col.a));
    screen.draw (ellipse);
    screen.display();
}