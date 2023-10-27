#include "buttons.h"

Button::Button (double x, double y, size_t w, size_t h):
    Widget (x, y, w, h),
    state (BTN_NORMAL)
    {}

void Button::MouseMove (const Vec& mousepos) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mousepos)) {
        if (state == BTN_NORMAL) {
            state = BTN_FOCUSED;
            Render (*GetRendertarget(), GetRegset());
        }
    }
    else {
        if (state == BTN_FOCUSED) {
            state = BTN_NORMAL;
            Render (*GetRendertarget(), GetRegset());
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

void ImgButton::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawTexture (textures[state], GetPos(), GetSize(), to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, rand() % 128 + 128, 0));
    #endif
}


TxtButton::TxtButton (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_):
    ImgButton (x, y, w, h, textures_),
    txt (txt_)
    {}

void TxtButton::SetText (const Text& txt_) {
    txt = txt_;    
}

void TxtButton::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawTexture (textures[state], GetPos(), GetSize(), to_draw);
    rt.DrawText (txt, GetPos() + Vec (40, 18), Color (0, 0, 0), to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 0, 128));
    #endif
}



BtnChooseMenu::BtnChooseMenu (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_):
    TxtButton (x, y, w, h, textures_, txt_),
    nextbtn_y (h)
    {}

void BtnChooseMenu::AddButton (Button* btn) {
    btn -> SetBounds (Rect(0, nextbtn_y, GetSize().x, btn -> GetSize().y));
    nextbtn_y += btn -> GetSize().y;
    AddSubWidget (btn);
}

void BtnChooseMenu::GetMaxRegset (RegionSet* dst) const {
    assert(dst != nullptr);
    if (state != BTN_PRESSED) dst -> AddRegion (GetBounds());
    else                      dst -> AddRegion (Rect(GetPos().x, GetPos().y, GetSize().x, nextbtn_y));
}

void BtnChooseMenu::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    if (state != BTN_PRESSED) return;

    if (mousebtn == MOUSE_LEFT && MouseOnWidget(mousepos)) {

        ListNode<Widget*>* node = GetSubwidgets() -> widgets.GetHead();
        ListNode<Widget*>* end_of_list = GetSubwidgets() -> widgets.EndOfList();
        while (node != end_of_list) {
            if (node -> val -> MouseOnWidget(mousepos)) node -> val -> MousePress(mousepos, mousebtn);
            else node -> val -> MouseRelease (mousepos, mousebtn);
            node = node -> next;
        }
    }
}

void BtnChooseMenu::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {}

void BtnChooseMenu::MouseMove (const Vec& mousepos) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mousepos)) {
        if (state == BTN_NORMAL) {
            state = BTN_PRESSED;
            Show();
            Render(*GetRendertarget(), GetRegset());
        }
        else {
            GetSubwidgets() -> MouseMove(mousepos);
        }
    }
    else {
        if (state == BTN_PRESSED) {
            state = BTN_NORMAL;
            Show();
            Render(*GetRendertarget(), GetRegset());
        }
    }
}

bool BtnChooseMenu::MouseOnWidget (const Vec& mousepos) const {
    if (GetRegset() -> Contains (mousepos)) return true;
    else if (state == BTN_PRESSED) return MouseOnSubwidgets (mousepos);
    else return false;
}
