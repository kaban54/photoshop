#include "rendertarget.h"

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

    inline sf::Color Color::GetSfColor() const {
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
    
    Texture::~Texture() {
        delete[] pixels;
    }
}


RenderTarget::RenderTarget (uint64_t w, uint64_t h):
    width (w),
    height (h),
    screen (),
    font ()
    {
        screen.create (w, h);
    }

inline void RenderTarget::SetFont (sf::Font* fnt) {
    font = fnt;
}

void RenderTarget::SfDisplay (sf::RenderWindow& sfwindow) const {   
    sf::Sprite sprite;
    sprite.setTexture (screen.getTexture());
    sprite.setPosition (0, 0);
    sfwindow.draw (sprite);
}

void RenderTarget::drawLine(Vec2 p1, Vec2 p2, Color color) {
    sf::Vertex line[] = {
        sf::Vertex (sf::Vector2f (p1.x, p1.y), color.GetSfColor()),
        sf::Vertex (sf::Vector2f (p2.x, p2.y), color.GetSfColor())
    };
    screen.draw (line, 2, sf::Lines);
    screen.display();
}

void RenderTarget::drawRect(Vec2 pos, Vec2 size, Color color) {
    sf::RectangleShape rectshape;
    rectshape.setFillColor (color.GetSfColor());

    rectshape.setSize (sf::Vector2f(size.x, size.y));
    rectshape.setPosition (pos.x, pos.y);
    screen.draw (rectshape);
    screen.display();
}

void RenderTarget::setPixel(Vec2 pos, Color color) {
    sf::Vertex pixel[] = {
        sf::Vertex (sf::Vector2f(pos.x, pos.y), color.GetSfColor())
    };
    screen.draw (pixel, 1, sf::Points);
    screen.display();
}

void RenderTarget::drawEllipse(Vec2 pos, Vec2 size, Color color) {
    sf::CircleShape ellipse (size.x / 2);
    ellipse.setPosition (pos.x, pos.y);
    ellipse.setScale (1, pos.y / pos.x);
    ellipse.setFillColor (color.GetSfColor());
    screen.draw (ellipse);
    screen.display();
}

void RenderTarget::drawTexture(Vec2 pos, Vec2 size, const Texture *texture) {
    sf::Image sfimg;
    sfimg.create(texture -> width, texture -> height, (uint8_t*)(texture -> pixels));
    sf::Texture sftexture;
    sftexture.loadFromImage(sfimg);
    sf::Sprite sfsprite(sftexture);
    sfsprite.setScale(size.x / texture -> width, size.y / texture -> height);
    sfsprite.setPosition(pos.x, pos.y);
    screen.draw(sfsprite);
    screen.display();
}

void RenderTarget::drawText(Vec2 pos, const char *content, uint16_t char_size, Color color) {

}

Texture* RenderTarget::getTexture() {
    sf::Image sfimg = screen.getTexture().copyToImage();
    Texture* ret = new Texture(sfimg.getSize().x, sfimg.getSize().y, (Color *)sfimg.getPixelsPtr());
}

void RenderTarget::display() {
    screen.display();
}

void RenderTarget::clear() {
    screen.clear();
    screen.display();
}

void RenderTarget::DrawRect_rs (const Rect& rect, const Color& col, const RegionSet* to_draw) {
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