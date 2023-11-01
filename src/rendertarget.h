#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <SFML/Graphics.hpp>
#include "region.h"
#include "vec.h"
#include "color.h"
#include "string"
#include "string.h"

struct Image {
    sf::Image sfimg;

    explicit Image() {}

    unsigned int GetWidth() const {return sfimg.getSize().x;}

    unsigned int GetHeight() const {return sfimg.getSize().y;}

    Color* GetPixels() const;
};

struct Texture {
    sf::Texture* sftexture;

    explicit Texture() {}

    explicit Texture (sf::Texture* sftexture_): sftexture (sftexture_) {}
};

struct Font {
    sf::Font* sffont;

    explicit Font (): sffont (nullptr) {}

    explicit Font (sf::Font* fnt_): sffont (fnt_) {}
};

struct Text {
    std::string str;
    Font fnt;
    size_t char_size;

    explicit Text (const std::string& str_, const Font& fnt_, size_t char_size_);

    Vec GetSize(size_t len) const;
};

class RenderTarget {
    unsigned int width;
    unsigned int height;
    sf::RenderTexture screen;

    public:

    RenderTarget (unsigned int w, unsigned int h);

    void GetImg (Image& img) const;

    void Display (sf::RenderWindow& window) const;

    void ClearScreen (const Color& col);

    void DrawRect (const Rect& rect, const Color& col, const RegionSet* to_draw = nullptr);

    void DrawLine (const Vec& p1, const Vec& p2, const Color& col, const RegionSet* to_draw = nullptr);

    void SetPixel (const Vec& point, const Color& col, const RegionSet* to_draw = nullptr);

    void DrawTexture (const Texture& texture, const Vec& pos, const Vec& size, const RegionSet* to_draw = nullptr);

    void DrawText (const Text& txt, const Vec& pos, const Color& col, const RegionSet* to_draw = nullptr);

    void DrawRenderTarget (const RenderTarget& rt, const Vec& pos, const RegionSet* to_draw = nullptr);

    void DrawCircle (const Vec& pos, double radius, const Color& col, const RegionSet* to_draw = nullptr);

    void DrawEllipse (const Rect& rect, const Color& col, const RegionSet* to_draw = nullptr);

    void DrawImg (const Image& img, const Vec& pos, const RegionSet* to_draw = nullptr);

    void DrawRegset (const RegionSet& regset, const Color& col, bool fill = false);
};

#endif