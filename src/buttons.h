#ifndef BUTTONS_H
#define BUTTONS_H

#include "widget.h"

enum ButtonState {
    BTN_NORMAL   = 0,
    BTN_PRESSED  = 1,
    BTN_FOCUSED  = 2,
    BTN_DISABLED = 3,
};

struct BtnArgs {};

typedef void BtnFunc (BtnArgs* args);

class Button : public Widget {
    BtnFunc* action;
    BtnArgs* action_args;
    public:

    ButtonState state;

    explicit Button (double x, double y, double w, double h, BtnFunc* action_, BtnArgs* action_args_);

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override {};
    virtual bool onKeyboardRelease (KeyboardContext context) override {};
    virtual bool onClock (uint64_t delta) override {};

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;

    virtual void render(RenderTargetI*) override;
};


// class ImgButton : public Button {
//     public:

//     Texture textures[4];

//     explicit ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_);

//     void SetTextures (const Texture* textures_);

//     virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
// };


class TxtButton : public Button {
    const char *txt;
    uint16_t char_size;
    static const uint16_t DEFAULT_CHAR_SIZE = 30;
    public:

    explicit TxtButton (double x, double y, double w, double h, BtnFunc* action_, BtnArgs* action_args_);

    void SetText (const char *str, uint16_t char_size_ = DEFAULT_CHAR_SIZE);

    virtual void RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) override;
};


void DrawButton (RenderTarget& rt, const Rect& rect, ButtonState state, const RegionSet* to_draw);

#endif