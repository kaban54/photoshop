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

    virtual void MouseMove (const Vec& mousepos) override;
};


class ImgButton : public Button {
    public:

    Texture textures[4];

    explicit ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_);

    void SetTextures (const Texture* textures_);

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;
};


class TxtButton : public ImgButton {
    Text txt;

    public:

    explicit TxtButton (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_);

    void SetText (const Text& txt);

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override {}

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override {}
};


class BtnChooseMenu : public TxtButton {
    Vec nextbtn_pos;
    public:

    explicit BtnChooseMenu(double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_);

    void AddButton (Button* btn);

    virtual void GetMaxRegset (RegionSet* dst) const override;

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) const override;
};

#endif