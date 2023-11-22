#include "menu.h"


bool Menu::onMousePress (MouseContext context) {
    if (getAvailable()) GetSubwidgets() -> onMousePress (context);
}

bool Menu::onMouseRelease (MouseContext context) {
    if (getAvailable()) GetSubwidgets() -> onMouseRelease (context);
}

bool Menu::onMouseMove (MouseContext context) {
    if (getAvailable()) GetSubwidgets() -> onMouseMove (context);
}

bool Menu::MouseOnWidget (const Vec2& mousepos) const {
    return MouseOnSubwidgets(mousepos);
}


MenuBtn::MenuBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_, Menu* menu_):
    TxtButton (x, y, w, h, nullptr, nullptr, str, char_size_),
    menu (menu_)
    {}

bool MenuBtn::onMousePress (MouseContext context) {
    if (!getAvailable()) return;
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (context.position) && state != BTN_PRESSED) {
        state = BTN_PRESSED;
        RenderInRegset (*GetRendertarget(), GetRegset());
        menu -> setAvailable(true);
        menu -> Show();
    }
}

bool MenuBtn::onMouseRelease (MouseContext context) {}

bool MenuBtn::onMouseMove (MouseContext context) {
    if (!getAvailable()) return;
    if (state == BTN_DISABLED) return;

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
        if (menu -> MouseOnWidget(context.position)) return;
        else {
            menu -> setAvailable(false);
            state = BTN_NORMAL;
            RenderInRegset (*GetRendertarget(), GetRegset());
            menu -> Show();
        }
    }
}


VerticalMenu::VerticalMenu (double x, double y):
    pos (Vec2(x, y)),
    nextbtn_y (y)
    {}

void VerticalMenu::AddButton (Button* btn) {
    btn -> SetBounds (Rect(pos.x, nextbtn_y, btn -> getSize().x, btn -> getSize().y));
    nextbtn_y += btn -> getSize().y;
    registerSubWidget (btn);
}
