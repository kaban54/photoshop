#include "buttons.h"

Button::Button (double x, double y, double w, double h, BtnFunc* action_, BtnArgs* action_args_):
    Widget (x, y, w, h),
    state (BTN_NORMAL),
    action (action_),
    action_args (action_args_)
    {}

void Button::MousePress (const MouseState& mstate) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mstate.pos)) {
        state = BTN_PRESSED;
        action(action_args);
        Render (*GetRendertarget(), GetRegset());
    }
}

void Button::MouseRelease (const MouseState& mstate) {
    if (state == BTN_PRESSED) {
        if (MouseOnWidget(mstate.pos)) state = BTN_FOCUSED;
        else                           state = BTN_NORMAL;

        Render (*GetRendertarget(), GetRegset());
    }
}

void Button::MouseMove (const MouseState& mstate) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mstate.pos)) {
        if (state == BTN_NORMAL) {
            state = BTN_FOCUSED;
            Render (*GetRendertarget(), GetRegset());
        }
    }
    else if (state == BTN_FOCUSED) {
        state = BTN_NORMAL;
        Render (*GetRendertarget(), GetRegset());
    }
}

void Button::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    DrawButton (rt, GetBounds(), state, to_draw);
}


// ImgButton::ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_):
//     Button (x, y, w, h)
//     {
//         if (textures_ != nullptr) {
//             textures[0] = textures_[0];
//             textures[1] = textures_[1];
//             textures[2] = textures_[2];
//             textures[3] = textures_[3];
//         }
//     }

// void ImgButton::SetTextures (const Texture* textures_) {
//     assert (textures != nullptr);
//     textures[0] = textures_[0];
//     textures[1] = textures_[1];
//     textures[2] = textures_[2];
//     textures[3] = textures_[3];
// }

// void ImgButton::Render (RenderTarget& rt, const RegionSet* to_draw) const {
//     rt.DrawTexture (textures[state], GetPos(), GetSize(), to_draw);

//     #ifdef REGDEBUG
//     rt.DrawRegset(*to_draw, Color(0, rand() % 128 + 128, 0));
//     #endif
// }


TxtButton::TxtButton (double x, double y, double w, double h, BtnFunc* action_, BtnArgs* action_args_, const Text& txt_):
    Button (x, y, w, h, action_, action_args_),
    txt (txt_)
    {}

void TxtButton::SetText (const Text& txt_) {
    txt = txt_;    
}

void TxtButton::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    DrawButton (rt, GetBounds(), state, to_draw);
    rt.DrawText (txt, GetPos() + Vec (40, 18), Color (0, 0, 0), to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 0, 128));
    #endif
}


BtnChooseMenu::BtnChooseMenu (double x, double y, double w, double h, const Text& txt_):
    TxtButton (x, y, w, h, nullptr, nullptr, txt_),
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

void BtnChooseMenu::MousePress (const MouseState& mstate) {
    if (state != BTN_PRESSED) return;

    if (mstate.btn == MOUSE_LEFT && MouseOnWidget(mstate.pos)) {

        ListNode<Widget*>* node = nullptr;
        GetSubwidgets() -> widgets.Iterate(node);
        while (node != nullptr) {
            if (node -> val -> MouseOnWidget(mstate.pos)) node -> val -> MousePress(mstate);
            else node -> val -> MouseRelease (mstate);
            GetSubwidgets() -> widgets.Iterate(node);
        }
    }
}

void BtnChooseMenu::MouseRelease (const MouseState& mstate) {}

void BtnChooseMenu::MouseMove (const MouseState& mstate) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mstate.pos)) {
        if (state == BTN_NORMAL) {
            state = BTN_PRESSED;
            Show();
            Render(*GetRendertarget(), GetRegset());
        }
        else {
            GetSubwidgets() -> MouseMove(mstate);
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

void DrawButton (RenderTarget& rt, const Rect& rect, ButtonState state, const RegionSet* to_draw) {
    if (state == BTN_NORMAL) {
        rt.DrawRect (rect, Color(128, 128, 128), to_draw);
        rt.DrawRect (Rect (rect.x, rect.y, rect.w - 4, 8)     , Color (192, 192, 192), to_draw);
        rt.DrawRect (Rect (rect.x, rect.y + 8, 8, rect.h - 12), Color (192, 192, 192), to_draw);
        rt.DrawRect (Rect (rect.x + 4, rect.Bot() - 8, rect.w - 4, 8)   , Color (64, 64, 64), to_draw);
        rt.DrawRect (Rect (rect.Right() - 8, rect.y + 4, 8, rect.h - 12), Color (64, 64, 64), to_draw);
        rt.DrawRect (Rect (rect.x + 4, rect.Bot() - 8, 4, 4)  , Color (128, 128, 128), to_draw);
        rt.DrawRect (Rect (rect.Right() - 8, rect.y + 4, 4, 4), Color (128, 128, 128), to_draw);
    }
    else if (state == BTN_FOCUSED) {
        rt.DrawRect (rect, Color(160, 160, 160), to_draw);
        rt.DrawRect (Rect (rect.x, rect.y, rect.w - 4, 8)     , Color (224, 224, 224), to_draw);
        rt.DrawRect (Rect (rect.x, rect.y + 8, 8, rect.h - 12), Color (224, 224, 224), to_draw);
        rt.DrawRect (Rect (rect.x + 4, rect.Bot() - 8, rect.w - 4, 8)   , Color (96, 96, 96), to_draw);
        rt.DrawRect (Rect (rect.Right() - 8, rect.y + 4, 8, rect.h - 12), Color (96, 96, 96), to_draw);
        rt.DrawRect (Rect (rect.x + 4, rect.Bot() - 8, 4, 4)  , Color (160, 160, 160), to_draw);
        rt.DrawRect (Rect (rect.Right() - 8, rect.y + 4, 4, 4), Color (160, 160, 160), to_draw);
    }
    else if (state == BTN_PRESSED) {
        rt.DrawRect (rect, Color (64, 64, 64), to_draw);
        rt.DrawRect (Rect (rect.x + 4, rect.y + 4, rect.w - 8, rect.h - 8), Color (128, 128, 128), to_draw);
    }
}