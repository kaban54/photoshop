#ifndef BUTTONS_H
#define BUTTONS_H

#include "widget.h"


class Button : public Widget {
    enum ButtonState {
        BTN_NORMAL   = 0,
        BTN_PRESSED  = 1,
        BTN_FOCUSED  = 2,
        BTN_DISABLED = 3,
    };

    public:
    size_t width;
    size_t height;
    ButtonState state;

    explicit Button (double x, double y, size_t w, size_t h);

    virtual void MouseMoveAction (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) override;

};


class BtnMenu : public Widget {
    Button** buttons;
    size_t size;
    size_t capacity;

    public:

    BtnMenu();

    ~BtnMenu();

    void AddButton (Button* btn);

    virtual void RenderThis (sf::RenderWindow& sfwindow) const;
};


class ImgButton : public Button {
    Texture textures[4];

    public:

    explicit ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_ = nullptr);

    void SetTextures (const Texture* textures_);

    virtual void Render (RenderTarget& screen, const RegionSet& to_draw) const override;


    virtual void MousePressAction (const Vec& mousepos, MouseButtons mousebtn) override {};

    virtual void MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) override {};
};

class TxtButton : public Button {
    Text txt;

    public:

    explicit TxtButton (double x, double y, size_t w, size_t h, const Text& txt_);

    void SetText (const Text& txt);

    virtual void Render (RenderTarget& screen, const RegionSet& to_draw) const override;


    virtual void MousePressAction (const Vec& mousepos, MouseButtons mousebtn) override {};

    virtual void MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) override {};
};

#endif