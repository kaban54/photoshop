#include "rendertarget.h"

RenderTarget::RenderTarget (uint64_t w, uint64_t h):
    width (w),
    height (h),
    screen (),
    font (),
    changed (false)
    {
        screen.create (w, h);
    }

void RenderTarget::SetFont (sf::Font* fnt) {
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
        sf::Vertex (sf::Vector2f (p1.x, p1.y), sf::Color(color.r, color.g, color.b, color.a)),
        sf::Vertex (sf::Vector2f (p2.x, p2.y), sf::Color(color.r, color.g, color.b, color.a))
    };
    screen.draw (line, 2, sf::Lines);
    screen.display();
}

void RenderTarget::drawRect(Vec2 pos, Vec2 size, Color color) {
    sf::RectangleShape rectshape;
    rectshape.setFillColor (sf::Color(color.r, color.g, color.b, color.a));

    rectshape.setSize (sf::Vector2f(size.x, size.y));
    rectshape.setPosition (pos.x, pos.y);
    screen.draw (rectshape);
    screen.display();
}

void RenderTarget::setPixel(Vec2 pos, Color color) {
    sf::Vertex pixel[] = {
        sf::Vertex (sf::Vector2f(pos.x, pos.y), sf::Color(color.r, color.g, color.b, color.a))
    };
    screen.draw (pixel, 1, sf::Points);
    screen.display();
}

void RenderTarget::drawEllipse(Vec2 pos, Vec2 size, Color color) {
    sf::CircleShape ellipse (size.x / 2);
    ellipse.setPosition (pos.x, pos.y);
    ellipse.setScale (1, size.y / size.x);
    ellipse.setFillColor (sf::Color(color.r, color.g, color.b, color.a));
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
    sf::Text text (content, *font, char_size);
    text.setPosition (pos.x, pos.y);
    text.setFillColor (sf::Color(color.r, color.g, color.b, color.a));
    screen.draw (text);
    screen.display();
}

Texture* RenderTarget::getTexture() {
    sf::Image sfimg = screen.getTexture().copyToImage();
    return new Texture(sfimg.getSize().x, sfimg.getSize().y, (Color *)sfimg.getPixelsPtr());
}

void RenderTarget::display() {
    changed = true;
    screen.display();
}

void RenderTarget::clear() {
    screen.clear();
    screen.display();
}

void RenderTarget::Fill(Color col) {
    screen.clear(sf::Color(col.r, col.g, col.b, col.a));
    screen.display();
}

Vec2 RenderTarget::GetTxtSize (const char* str, size_t char_size, size_t len) const {
    char substr[len + 1] = "";
    strncpy(substr, str, len);
    substr[len] = '\0';
    sf::Text txt (substr, *font, char_size);
    sf::FloatRect bounds = txt.getGlobalBounds();
    return Vec2(bounds.width, bounds.height);
}

void RenderTarget::DrawRect_rs (const Rect& rect, Color col, const RegionSet* to_draw) {
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

void RenderTarget::DrawText_rs (Vec2 pos, const char *content, uint16_t char_size, Color col, const RegionSet* to_draw) {
    sf::Text text (content, *font, char_size);
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

    Rect txtrect (bounds.left, bounds.top, bounds.width, bounds.height);
    txtrect.Move (pos);

    RegionSet newregset;
    newregset.AddRegion (txtrect);
    newregset ^= *to_draw;

    ListNode<Rect>* end_of_list = newregset.regions.EndOfList();
    ListNode<Rect>* node = newregset.regions.GetHead();

    while (node != end_of_list) {
        Rect region = node -> val;

        double x = region.x - txtrect.Left();
        double y = region.y - txtrect.Top();
        double w = region.w;
        double h = region.h;

        sprite.setPosition (region.x, region.y);
        sprite.setTextureRect (sf::IntRect(x, y, w, h));
        screen.draw (sprite);
        screen.display();

        node = node -> next;
    }
}

void RenderTarget::DrawTexture_rs (Vec2 pos, Vec2 size, const Texture *texture, const RegionSet* to_draw) {
    sf::Image sfimg;
    sfimg.create(texture -> width, texture -> height, (uint8_t *)texture -> pixels);
    sf::Texture sftexture;
    sftexture.loadFromImage(sfimg);

    sf::Sprite sprite;
    sprite.setTexture (sftexture);

    double xscale = size.x / texture -> width;
    double yscale = size.y / texture -> height;
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

void RenderTarget::DrawRenderTarget_rs (const RenderTarget& rt, const Vec2& pos, const RegionSet* to_draw) {
    sf::Sprite sprite;
    sprite.setTexture (rt.screen.getTexture());

    // std::cerr << rt.width << " " << rt.height << "\n";

    RegionSet tmp, rects;
    tmp.AddRegion (Rect(pos, pos + Vec2(rt.width, rt.height)));
    IntersectRegsets (tmp, *to_draw, rects);
    ListNode<Rect>* end_of_list = rects.regions.EndOfList();
    ListNode<Rect>* node = rects.regions.GetHead();


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

void RenderTarget::DrawLine_rs (Vec2 p1, Vec2 p2, double thikness, Color col, const RegionSet* to_draw) {
    sf::Color color (col.r, col.g, col.b, col.a);
    if (thikness == 1) {
        sf::Vertex line[] = {
            sf::Vertex (sf::Vector2f (p1.x, p1.y), color),
            sf::Vertex (sf::Vector2f (p2.x, p2.y), color)
        };
        screen.draw (line, 2, sf::Lines);
    }
    else {
        Vec2 dir = p2 - p1;
        Vec2 normal (-dir.y, dir.x);
        normal = !normal * thikness / 2;

        sf::ConvexShape polygon;
        polygon.setFillColor (color);
        polygon.setPointCount (4);
        polygon.setPoint (0, sf::Vector2f(p1.x + normal.x, p1.y + normal.y));
        polygon.setPoint (1, sf::Vector2f(p1.x - normal.x, p1.y - normal.y));
        polygon.setPoint (2, sf::Vector2f(p2.x - normal.x, p2.y - normal.y));
        polygon.setPoint (3, sf::Vector2f(p2.x + normal.x, p2.y + normal.y));
        screen.draw(polygon);
    }
    screen.display();
}
