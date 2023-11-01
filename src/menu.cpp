#include "menu.h"


void Menu::MousePress (const MouseState& mstate) {
    if (visible) GetSubwidgets() -> MousePress (mstate);
}

void Menu::MouseRelease (const MouseState& mstate) {
    if (visible) GetSubwidgets() -> MouseRelease (mstate);
}

void Menu::MouseMove (const MouseState& mstate) {
    if (visible) GetSubwidgets() -> MouseMove (mstate);
}


MenuBtn::MenuBtn (double x, double y, double w, double h, const Text& txt_, Menu* menu_):
    TxtButton (x, y, w, h, nullptr, nullptr, txt_),
    menu (menu_)
    {}

void MenuBtn::MousePress (const MouseState& mstate) {
    if (!visible) return;
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mstate.pos) && state != BTN_PRESSED) {
        state = BTN_PRESSED;
        Render (*GetRendertarget(), GetRegset());
        menu -> visible = true;
        menu -> Show();
    }
}

void MenuBtn::MouseRelease (const MouseState& mstate) {}

void MenuBtn::MouseMove (const MouseState& mstate) {
    if (!visible) return;
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
    else if (state == BTN_PRESSED) {
        if (menu -> MouseOnSubwidgets(mstate.pos)) return;
        else {
            menu -> visible = false;
            state = BTN_NORMAL;
            Render (*GetRendertarget(), GetRegset());
            menu -> Show();
        }
    }
}


VerticalMenu::VerticalMenu (double x, double y):
    pos (Vec(x, y)),
    nextbtn_y (y)
    {}

void VerticalMenu::AddButton (Button* btn) {
    btn -> SetBounds (Rect(pos.x, nextbtn_y, btn -> GetSize().x, btn -> GetSize().y));
    nextbtn_y += btn -> GetSize().y;
    AddSubWidget (btn);
}
