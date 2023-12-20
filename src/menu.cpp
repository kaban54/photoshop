#include "menu.h"


bool Menu::onMousePress (MouseContext context) {
    if (getAvailable()) GetSubwidgets() -> onMousePress (context);
    return false;
}

bool Menu::onMouseRelease (MouseContext context) {
    if (getAvailable()) GetSubwidgets() -> onMouseRelease (context);
    return false;
}

bool Menu::onMouseMove (MouseContext context) {
    if (getAvailable()) GetSubwidgets() -> onMouseMove (context);
    return false;
}

bool Menu::MouseOnWidget (const Vec2& mousepos) const {
    return MouseOnSubwidgets(mousepos);
    return false;
}


MenuBtn::MenuBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_, Menu* menu_):
    TxtButton (x, y, w, h, nullptr, nullptr, str, char_size_),
    menu (menu_)
    {}

bool MenuBtn::onMousePress (MouseContext context) {
    if (!getAvailable() || !GetVisible()) return false;
    if (state == BTN_DISABLED) return false;

    if (MouseOnWidget (context.position) && state != BTN_PRESSED) {
        state = BTN_PRESSED;
        RenderInRegset (*GetRendertarget(), GetRegset());
        menu -> SetVisible(true);
        menu -> Show();
    }
    return false;
}

bool MenuBtn::onMouseRelease (MouseContext context) {return false;}

bool MenuBtn::onMouseMove (MouseContext context) {
    if (!getAvailable() || !GetVisible()) return false;
    if (state == BTN_DISABLED) return false;

    GetSubwidgets() -> onMouseMove(context);
    menu -> onMouseMove(context);

    if (MouseOnWidget (context.position)) {
        if (state == BTN_NORMAL) {
            state = BTN_FOCUSED;
            RenderInRegset (*GetRendertarget(), GetRegset());
        }
    }
    else if (state == BTN_FOCUSED) {
        state = BTN_NORMAL;
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    else if (state == BTN_PRESSED) {
        if (menu -> MouseOnWidget(context.position)) return false;
        else {
            menu -> SetVisible(false);
            state = BTN_NORMAL;
            RenderInRegset (*GetRendertarget(), GetRegset());
            menu -> Show();
        }
    }
    return false;
}


VerticalMenu::VerticalMenu (double x, double y):
    pos (Vec2(x, y)),
    nextbtn_y (y),
    basenextbtn_y (y)
    {}

void VerticalMenu::AddButton (Button* btn) {
    btn -> SetBounds (Rect(pos.x, nextbtn_y, btn -> getSize().x, btn -> getSize().y));
    nextbtn_y += btn -> getSize().y;
    registerSubWidget (btn);
}

void VerticalMenu::Reset() {
    GetSubwidgets() -> widgets.Clear();
    nextbtn_y = basenextbtn_y;
}

TwoColMenu::TwoColMenu (double x, double y, double w, double h):
    pos (x, y),
    btn_size (w, h),
    nextbtn_pos (x, y)
    {}

void TwoColMenu::AddButton (Button* btn) {
    btn -> SetBounds(Rect(nextbtn_pos, nextbtn_pos + btn_size));
    if (nextbtn_pos.x == pos.x) nextbtn_pos.x += btn_size.x;
    else {
        nextbtn_pos.x -= btn_size.x;
        nextbtn_pos.y += btn_size.y;
    }
    registerSubWidget (btn);
}