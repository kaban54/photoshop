#ifndef BUTTONS_H
#define BUTTONS_H

#include "widget.h"

enum ButtonState {
    BTN_NORMAL   = 0,
    BTN_PRESSED  = 1,
    BTN_FOCUSED  = 2,
    BTN_DISABLED = 3,
};

class Button : public Widget {
    public:
    ButtonState state;

    explicit Button (double x, double y, size_t w, size_t h);

    virtual void MouseMove (const MouseState& mstate) override;

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
};


class ImgButton : public Button {
    public:

    Texture textures[4];

    explicit ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_);

    void SetTextures (const Texture* textures_);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
};


class TxtButton : public Button {
    Text txt;

    public:

    explicit TxtButton (double x, double y, size_t w, size_t h, const Text& txt_);

    void SetText (const Text& txt);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void MousePress (const MouseState& mstate) override {}

    virtual void MouseRelease (const MouseState& mstate) override {}
};


class BtnChooseMenu : public TxtButton {
    double nextbtn_y;
    public:

    explicit BtnChooseMenu(double x, double y, size_t w, size_t h, const Text& txt_);

    void AddButton (Button* btn);

    virtual void GetMaxRegset (RegionSet* dst) const override;

    //virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual bool MouseOnWidget (const Vec& mousepos) const override;
};


void DrawButton (RenderTarget& rt, const Rect& rect, ButtonState state, const RegionSet* to_draw);

#endif