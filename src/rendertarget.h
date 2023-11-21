#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <SFML/Graphics.hpp>
#include <cinttypes>
#include "region.h"
#include "vec2.h"
#include "string"
#include "string.h"

namespace plugin {
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        explicit Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255);

        explicit Color();

        sf::Color GetSfColor() const;
    };

    struct Texture {
        uint64_t height;
        uint64_t width;

        Color *pixels;

        explicit Texture(uint64_t w, uint64_t h, const Color* pix = nullptr);

        ~Texture();
    };

    struct RenderTargetI {
        virtual void setPixel(Vec2 pos, Color color) = 0;
        virtual void drawLine(Vec2 pos, Vec2 point1, Color color) = 0;
        virtual void drawRect(Vec2 pos, Vec2 size, Color color) = 0;
        virtual void drawEllipse(Vec2 pos, Vec2 size, Color color) = 0;
        virtual void drawTexture(Vec2 pos, Vec2 size, const Texture *texture) = 0;
        virtual void drawText(Vec2 pos, const char *content, uint16_t char_size, Color color) = 0;

        virtual Texture *getTexture() = 0;

        /// как в RenderTexture::display
        virtual void display() = 0;

        /// clear
        virtual void clear() = 0;
    };
}


using namespace plugin;

class RenderTarget : public RenderTargetI {
    uint64_t width;
    uint64_t height;
    sf::RenderTexture screen;
    sf::Font* font;

    public:

    explicit RenderTarget (uint64_t w, uint64_t h);

    void SetFont(sf::Font* fnt);

    void SfDisplay (sf::RenderWindow& sfwindow) const;

    virtual void setPixel(Vec2 pos, Color color) override;

    virtual void drawLine(Vec2 p1, Vec2 p2, Color color) override;

    virtual void drawRect(Vec2 pos, Vec2 size, Color color) override;

    virtual void drawEllipse(Vec2 pos, Vec2 size, Color color) override;

    virtual void drawTexture(Vec2 pos, Vec2 size, const Texture *texture) override;

    virtual void drawText(Vec2 pos, const char *content, uint16_t char_size, Color color) override;

    virtual Texture *getTexture() override;

    virtual void display() override;

    virtual void clear() override;

    void DrawRect_rs (const Rect& rect, const Color& col, const RegionSet* to_draw);
};

#endif