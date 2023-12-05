#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <cinttypes>
#include <cstring>
#include "region.h"
#include "vec2.h"
#include "texture.h"
#include <SFML/Graphics.hpp>

namespace plugin {
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
    Texture* texture;
    public:
    bool changed;

    explicit RenderTarget (uint64_t w, uint64_t h);

    Vec2 GetSize() const {return Vec2(width, height);}

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

    void Fill(Color col);

    Vec2 GetTxtSize (const char* str, size_t char_size, size_t len) const;

    void DrawRect_rs (const Rect& rect, Color col, const RegionSet* to_draw);

    void DrawText_rs (Vec2 pos, const char *content, uint16_t char_size, Color col, const RegionSet* to_draw);

    void DrawTexture_rs (Vec2 pos, Vec2 size, const Texture *texture, const RegionSet* to_draw);

    void DrawRenderTarget_rs (const RenderTarget& rt, const Vec2& pos, const RegionSet* to_draw);

    void DrawLine_rs (Vec2 p1, Vec2 p2, double thikness, Color col, const RegionSet* to_draw);
};

#endif