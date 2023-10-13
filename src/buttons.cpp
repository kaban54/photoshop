#include "buttons.h"

Button::Button (double x, double y, size_t w, size_t h):
    Widget (x, y, w, h, 0),
    width (w),
    height (h),
    state (BTN_NORMAL)
    {}


bool Button::MouseOnWidget (const Vec& mousepos) {
    return (mousepos.x >= pos.x && mousepos.x <= pos.x + width ) &&
           (mousepos.y >= pos.y && mousepos.y <= pos.y + height);
}

void Button::MouseMoveAction (const Vec& mousepos) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mousepos)) {
        if (state == BTN_NORMAL) {
            state = BTN_FOCUSED;
            Render (*rt, regset);
            std::cerr << regset.regions.GetSize() << "\n";
        }
    }
    else {
        if (state == BTN_FOCUSED) {
            state = BTN_NORMAL;
            Render (*rt, regset);
        }
    }
}


ImgButton::ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_):
    Button (x, y, w, h)
    {
        if (textures_ != nullptr) {
            textures[0] = textures_[0];
            textures[1] = textures_[1];
            textures[2] = textures_[2];
            textures[3] = textures_[3];
        }
    }

void ImgButton::SetTextures (const Texture* textures_) {
    assert (textures != nullptr);

    textures[0] = textures_[0];
    textures[1] = textures_[1];
    textures[2] = textures_[2];
    textures[3] = textures_[3];
}


void ImgButton::Render (RenderTarget& screen, const RegionSet& to_draw) const {
    screen.DrawTexture (textures[state], pos, Vec(width, height), to_draw);
}


/*
TxtButton::TxtButton (double x, double y, size_t w, size_t h, const Text& txt_):
    Button (x, y, w, h),
    txt (txt_)
    {
        regset.AddRegion (Rect(Vec(x, y), Vec(x + w, y + h)));
    }

void TxtButton::SetText (const Text& txt_) {
    
}*/