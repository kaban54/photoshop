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

    virtual bool MouseOnWidget (const Vec& mousepos) const override;

};


class ImgButton : public Button {
    Texture textures[4];

    public:

    explicit ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_ = nullptr);

    void SetTextures (const Texture* textures_);

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;

    virtual void MousePress (const Vec& mousepos, MouseButtons mousebtn) override {};

    virtual void MouseRelease (const Vec& mousepos, MouseButtons mousebtn) override {};
};


class TxtButton : public Button {
    Text txt;

    public:

    explicit TxtButton (double x, double y, size_t w, size_t h, const Text& txt_);

    void SetText (const Text& txt);

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;

    virtual void MousePress (const Vec& mousepos, MouseButtons mousebtn) override {std::cout << "p\n";}

    virtual void MouseRelease (const Vec& mousepos, MouseButtons mousebtn) override {}
};


class BtnMenu : public Widget {
    Button* show_btn;

    public:

    BtnMenu(Button* show_btn_);

    ~BtnMenu();

    void AddButton (Button* btn);

    virtual void Move (const Vec& vec) override;

    virtual void MousePress (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) const override;

    virtual void Render (RenderTarget& rt, RegionSet* to_draw) const override;
};

#endif