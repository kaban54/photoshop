#include "window.h"

Window::Window (double x, double y, double w, double h, bool close_btn):
    Widget (x, y, w, h),
    is_moving (false),
    need_to_close (false)
    {
        if (close_btn) registerSubWidget (new WindowCloseBtn (w - 30, 0, 30, 20, this));
    }

void Window::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    Rect rect = GetBounds();
    rt.DrawRect_rs (rect, Color(0, 0, 0), to_draw);
    rect.x += 2;
    rect.y += 20;
    rect.w -= 4;
    rect.h -= 22;
    rt.DrawRect_rs (rect, WINDOW_BG_COLOR, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(255, 0, 0, 128));
    #endif
}

bool Window::onMousePress (MouseContext context) {
    if (!getAvailable()) return false;
    if (MouseOnWidget(context.position)) {
        if (Rect(getPos().x, getPos().y, getSize().x, 20).Contains(context.position) &&
            context.button == MouseButton::Left) {
                is_moving = true;
                hold_pos = context.position;
            }
        Show();
    }
    GetSubwidgets() -> onMousePress(context);
    if (need_to_close) Close();
    return false;
}

bool Window::onMouseRelease (MouseContext context) {
    if (!getAvailable()) return false;
    if (context.button == MouseButton::Left && is_moving) {
        is_moving = false;
        hold_pos = Vec2 (0, 0);
    }
    GetSubwidgets() -> onMouseRelease (context);
    return false;
}

bool Window::onMouseMove (MouseContext context) {
    if (!getAvailable()) return false;
    if (is_moving) {
        Vec2 mousepos = context.position;
        if (mousepos.x != hold_pos.x || mousepos.y != hold_pos.y) {
            move (mousepos - hold_pos);
            Show();
            RenderInRegset(*GetRendertarget(), GetRegset());
            RenderSubWidgets(*GetRendertarget());
            hold_pos = mousepos;
        }
    }
    GetSubwidgets() -> onMouseMove (context);
    return false;
}

bool Window::onKeyboardPress (KeyboardContext context) {
    GetSubwidgets() -> onKeyboardPress (context);
    return false;
}

bool Window::onKeyboardRelease (KeyboardContext context) {
    GetSubwidgets() -> onKeyboardRelease (context);
    return false;
}

bool Window::onClock (uint64_t delta) {
    GetSubwidgets() -> onClock(delta);
    return false;
}



Background::Background (double w_, double h_):
    Widget (0, 0, w_, h_)
    {}

void Background::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    rt.DrawRect_rs (GetBounds(), BG_COLOR, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 0, 255, 128));
    #endif
}

bool Background::onMousePress (MouseContext context) {
    GetSubwidgets() -> onMousePress (context);
    return false;
}

bool Background::onMouseRelease (MouseContext context) {
    GetSubwidgets() -> onMouseRelease (context);
    return false;
}

bool Background::onMouseMove (MouseContext context) {
    GetSubwidgets() -> onMouseMove (context);
    return false;
}

bool Background::onKeyboardPress (KeyboardContext context) {
    GetSubwidgets() -> onKeyboardPress (context);
    return false;
}

bool Background::onKeyboardRelease (KeyboardContext context) {
    GetSubwidgets() -> onKeyboardRelease (context);
    return false;
}

bool Background::onClock (uint64_t delta) {
    GetSubwidgets() -> onClock (delta);
    return false;
}


ModalWindow::ModalWindow (double x, double y, double w, double h, EventManagerI* event_man_):
    Window (x, y, w, h, false),
    event_man (event_man_)
    {
        event_man -> registerObject (this);
        event_man -> setPriority (EventType::MousePress  , 1);
        event_man -> setPriority (EventType::MouseRelease, 1);
        event_man -> setPriority (EventType::MouseMove   , 1);
        event_man -> setPriority (EventType::KeyPress    , 1);
        event_man -> setPriority (EventType::KeyRelease  , 1);
    }

ModalWindow::~ModalWindow() {
    event_man -> setPriority (EventType::MousePress  , 0);
    event_man -> setPriority (EventType::MouseRelease, 0);
    event_man -> setPriority (EventType::MouseMove   , 0);
    event_man -> setPriority (EventType::KeyPress    , 0);
    event_man -> setPriority (EventType::KeyRelease  , 0);
    event_man -> unregisterObject (this);
}

void window_close_btn_action (BtnArgs* btn_args) {
    ((WindowCloseBtnArgs*) btn_args) -> win -> need_to_close = true;
}

WindowCloseBtn::WindowCloseBtn (double x, double y, double w, double h, Window* win_):
    Button (x, y, w, h, window_close_btn_action, &wclose_btn_args),
    wclose_btn_args (win_)
    {}

void WindowCloseBtn::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    rt.DrawRect_rs (GetBounds(), Color(255, 0, 0), to_draw);
}
