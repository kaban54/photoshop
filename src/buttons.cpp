#include "buttons.h"

Button::Button (double x, double y, size_t w, size_t h):
    Widget (x, y, w, h),
    state (BTN_NORMAL)
    {}


bool Button::MouseOnWidget (const Vec& mousepos) const {
    return Rect(pos, pos + size).Contains(mousepos);
}

void Button::MouseMove (const Vec& mousepos) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mousepos)) {
        if (state == BTN_NORMAL) {
            state = BTN_FOCUSED;
            Render (*rt, &regset);
        }
    }
    else {
        if (state == BTN_FOCUSED) {
            state = BTN_NORMAL;
            Render (*rt, &regset);
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

void ImgButton::Render (RenderTarget& rt, RegionSet* to_draw) const {
    rt.DrawTexture (textures[state], pos, size, to_draw);
}


TxtButton::TxtButton (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_):
    ImgButton (x, y, w, h, textures_),
    txt (txt_)
    {}

void TxtButton::SetText (const Text& txt_) {
    txt = txt_;    
}

void TxtButton::Render (RenderTarget& rt, RegionSet* to_draw) const {
    rt.DrawTexture (textures[state], pos, size, to_draw);
    rt.DrawText (txt, pos + Vec (40, 18), Color (0, 0, 0), to_draw);
}



BtnChooseMenu::BtnChooseMenu (Button* show_btn_):
    Widget (show_btn_ -> pos.x, show_btn_ -> pos.y, show_btn_ -> size.x, show_btn_ -> size.y),
    show_btn (show_btn_)
    {}

BtnChooseMenu::~BtnChooseMenu() {
    delete (show_btn);
}

void BtnChooseMenu::AddButton (Button* btn) {
    btn -> pos = pos + Vec (0, size.y);
    btn -> size.x = size.x;
    size.y += btn -> size.y;
    subwidgets.AddWidget (btn);
}

void BtnChooseMenu::Move (const Vec& vec) {
    show_btn -> Move (vec);
    pos += vec;
    regset.Move(vec);
    subwidgets.Move(vec);
}

void BtnChooseMenu::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    if (show_btn -> state != BTN_PRESSED) return;

    if (mousebtn == MOUSE_LEFT && MouseOnWidget(mousepos) && !show_btn -> MouseOnWidget(mousepos)) {
        
        ListNode<Widget*>* node = subwidgets.widgets.GetHead();
        while (node != subwidgets.widgets.EndOfList()) {
            if (node -> val -> MouseOnWidget(mousepos)) node -> val -> MousePress(mousepos, mousebtn);
            node = node -> next;
        }

        
        for (size_t i = 0; i < subwidgets.GetSize(); i++) {
            if (subwidgets[i] -> MouseOnWidget(mousepos)) {
                subwidgets[i] -> MousePress(mousepos, mousebtn);
            }
            else subwidgets[i] -> MouseRelease(mousepos, mousebtn);
        }


    }
}

void BtnChooseMenu::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {}

void BtnChooseMenu::MouseMove (const Vec& mousepos) {
    if (show_btn -> state == BTN_DISABLED) return;

    if (MouseOnWidget (mousepos)) {
        if (show_btn -> state == BTN_NORMAL) {
            show_btn -> state = BTN_PRESSED;
            Render (*rt, &regset);
        }
    }
    else {
        if (show_btn -> state == BTN_PRESSED) {
            show_btn -> state = BTN_NORMAL;
            Render (*rt, &regset);
        }
    }
    if (show_btn -> state == BTN_PRESSED) {
        subwidgets.MouseMove(mousepos);
    }
}

bool BtnChooseMenu::MouseOnWidget (const Vec& mousepos) const {
    if (show_btn -> MouseOnWidget(mousepos)) return true;
    if (show_btn -> state != BTN_PRESSED) return false;

    for (size_t i = 0; i < subwidgets.GetSize(); i++) {
        if (subwidgets[i] -> MouseOnWidget(mousepos)) return true;
    }
    return false;
}

void BtnChooseMenu::Render (RenderTarget& rt, RegionSet* to_draw) const {
    show_btn -> Render (rt, to_draw);
    if (show_btn -> state == BTN_PRESSED) {
        for (size_t i = 0; i < subwidgets.GetSize(); i++) {
            subwidgets[i] -> Render (rt, to_draw);
        }
    }
}