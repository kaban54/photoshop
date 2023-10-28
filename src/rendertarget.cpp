#include "rendertarget.h"

Color* Image::GetPixels() const {
    return (Color*)sfimg.getPixelsPtr();
}

RenderTarget::RenderTarget (unsigned int w, unsigned int h):
    width (w),
    height (h),
    screen ()
    {
        screen.create (w, h);
    }

void RenderTarget::GetImg (Image& img) const {
    img.sfimg = screen.getTexture().copyToImage();
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

void RenderTarget::DrawRect (const Rect& rect, const Color& col, const RegionSet* to_draw) {
    sf::RectangleShape rectshape;
    rectshape.setFillColor (sf::Color (col.r, col.g, col.b));

    if (to_draw == nullptr) {
        rectshape.setSize (sf::Vector2f(rect.w, rect.h));
        rectshape.setPosition (rect.x, rect.y);
        screen.draw (rectshape);
        screen.display();
    }
    else {
        RegionSet tmp, rects;
        tmp.AddRegion (rect);
        IntersectRegsets (tmp, *to_draw, rects);
        ListNode<Rect>* end_of_list = rects.regions.EndOfList();
        ListNode<Rect>* node = rects.regions.GetHead();

        while (node != end_of_list) {
            Rect region = node -> val;

            rectshape.setSize (sf::Vector2f(region.w, region.h));
            rectshape.setPosition (region.x, region.y);
            screen.draw (rectshape);
            screen.display();

            node = node -> next;
        }
    }
}

void RenderTarget::DrawLine (const Vec& p1, const Vec& p2, const Color& col, const RegionSet* regset) {    
    sf::Color color (col.r, col.g, col.b, col.a);
    sf::Vertex line[] = {
        sf::Vertex (sf::Vector2f (p1.x, p1.y), color),
        sf::Vertex (sf::Vector2f (p2.x, p2.y), color)
    };
    screen.draw (line, 2, sf::Lines);
    screen.display();
}

void RenderTarget::SetPixel (const Vec& point, const Color& col, const RegionSet* regset) {
    sf::Color color (col.r, col.g, col.b, col.a);
    sf::Vertex pixel[] = {
        sf::Vertex (sf::Vector2f(point.x, point.y), color)
    };
    screen.draw (pixel, 1, sf::Points);
    screen.display();
}

void RenderTarget::DrawTexture (const Texture& texture, const Vec& pos, const Vec& size, const RegionSet* to_draw) {
    sf::Sprite sprite;
    sprite.setTexture (*(texture.sftexture));

    double xscale = size.x / texture.sftexture -> getSize().x;
    double yscale = size.y / texture.sftexture -> getSize().y;
    sprite.setScale (xscale, yscale);

    if (to_draw == nullptr) {
        sprite.setPosition (pos.x, pos.y);
        screen.draw (sprite);
        screen.display();
    }
    else {
        ListNode<Rect>* end_of_list = to_draw -> regions.EndOfList();
        ListNode<Rect>* node = to_draw -> regions.GetHead();

        while (node != end_of_list) {
            Rect region = node -> val;

            double x = (region.x - pos.x) / xscale;
            double y = (region.y - pos.y) / yscale;
            double w = region.w  / xscale;
            double h = region.h / yscale;

            sprite.setPosition (region.x, region.y);
            sprite.setTextureRect (sf::IntRect(x, y, w, h));
            screen.draw (sprite);
            screen.display();

            node = node -> next;
        }
    }
}

void RenderTarget::DrawText (const Text& txt, const Vec& pos, const Color& col, const RegionSet* regset) {
    sf::Text text = *(txt.sftxt);
    text.setPosition (0, 0);
    sf::FloatRect bounds = text.getGlobalBounds();
    sf::RenderTexture sfrt;
    sfrt.create(bounds.width, bounds.height);
    sfrt.clear (sf::Color::Transparent);
    text.setFillColor (sf::Color (col.r, col.g, col.b, col.a));
    text.setPosition (-bounds.left, -bounds.top);
    sfrt.draw (text);
    sfrt.display();

    sf::Sprite sprite;
    sprite.setTexture (sfrt.getTexture());

    Rect txtrect (Vec(), Vec(bounds.width, bounds.height));
    txtrect.Move (pos);

    RegionSet newregset;
    newregset.AddRegion (txtrect);
    newregset ^= *regset;

    ListNode<Rect>* end_of_list = newregset.regions.EndOfList();
    ListNode<Rect>* node = newregset.regions.GetHead();

    while (node != end_of_list) {
        Rect region = node -> val;

        double x = region.x - pos.x;
        double y = region.y - pos.y;
        double w = region.w;
        double h = region.h;

        sprite.setPosition (region.x, region.y);
        sprite.setTextureRect (sf::IntRect(x, y, w, h));
        screen.draw (sprite);
        screen.display();

        node = node -> next;
    }
}

void RenderTarget::DrawRenderTarget (const RenderTarget& rt, const Vec& pos, const RegionSet* regset) {
    sf::Sprite sprite;
    sprite.setTexture (rt.screen.getTexture());

    ListNode<Rect>* end_of_list = regset -> regions.EndOfList();
    ListNode<Rect>* node = regset -> regions.GetHead();

    while (node != end_of_list) {
        Rect region = node -> val;

        double x = region.x - pos.x;
        double y = region.y - pos.y;
        double w = region.w;
        double h = region.h;

        sprite.setPosition (region.x, region.y);
        sprite.setTextureRect (sf::IntRect(x, y, w, h));
        screen.draw (sprite);
        screen.display();

        node = node -> next;
    }
}

void RenderTarget::DrawCircle (const Vec& pos, double radius, const Color& col, const RegionSet* to_draw) {
    sf::CircleShape circle (radius);
    circle.setPosition (pos.x - radius, pos.y - radius);
    circle.setFillColor (sf::Color(col.r, col.g, col.b, col.a));
    screen.draw (circle);
    screen.display();
}

void RenderTarget::DrawEllipse (const Rect& rect, const Color& col, const RegionSet* to_draw) {
    sf::CircleShape ellipse (rect.w / 2);
    ellipse.setPosition (rect.x, rect.y);
    ellipse.setScale (1, rect.h / rect.w);
    ellipse.setFillColor (sf::Color(col.r, col.g, col.b, col.a));
    screen.draw (ellipse);
    screen.display();
}

void RenderTarget::DrawImg (const Image& img, const Vec& pos, const RegionSet* to_draw) {
    sf::Texture sftexture;
    sftexture.loadFromImage (img.sfimg);
    Texture texture (&sftexture);
    DrawTexture (texture, pos, Vec(img.GetWidth(), img.GetHeight()), to_draw);
    screen.display();
}

void RenderTarget::DrawRegset (const RegionSet& regset, const Color& col) {
    sf::RectangleShape rect;
    const int outl_thikness = 3;
    rect.setFillColor (sf::Color::Transparent);
    rect.setOutlineThickness (outl_thikness);
    rect.setOutlineColor (sf::Color(col.r, col.g, col.b, col.a));

    ListNode<Rect>* end_of_list = regset.regions.EndOfList();
    ListNode<Rect>* node = regset.regions.GetHead();
    while (node != end_of_list) {
        Rect region = node -> val;
        rect.setSize (sf::Vector2f (region.w - outl_thikness * 2, region.h - outl_thikness * 2));
        rect.setPosition (region.x + outl_thikness, region.y + outl_thikness);
        screen.draw (rect);
        screen.display();

        node = node -> next;
    }
}