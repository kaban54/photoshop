#include "window.h"

Window::Window (double x, double y, double w, double h, bool close_btn):
    Widget (x, y, w, h),
    is_moving (false),
    need_to_close (false)
    {
        if (close_btn) AddSubWidget (new WindowCloseBtn (w - 30, 0, 30, 20, this));
    }

void Window::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    Rect rect = GetBounds();
    rt.DrawRect (rect, Color(0, 0, 0), to_draw);
    rect.x += 2;
    rect.y += 20;
    rect.w -= 4;
    rect.h -= 22;
    rt.DrawRect (rect, WINDOW_BG_COLOR, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(255, 0, 0, 128));
    #endif
}

void Window::MousePress (const MouseState& mstate) {
    if (!visible) return;
    if (MouseOnWidget(mstate.pos)) {
        if (Rect(GetPos().x, GetPos().y, GetSize().x, 20).Contains(mstate.pos) && mstate.btn == MOUSE_LEFT) {
                is_moving = true;
                hold_pos = mstate.pos;
            }
        Show();
    }
    GetSubwidgets() -> MousePress (mstate);
    if (need_to_close) Close();
}

void Window::MouseRelease (const MouseState& mstate) {
    if (!visible) return;
    if (mstate.btn == MOUSE_LEFT && is_moving) {
        is_moving = false;
        hold_pos = Vec (0, 0);
    }
    GetSubwidgets() -> MouseRelease (mstate);
}

void Window::MouseMove (const MouseState& mstate) {
    if (!visible) return;
    if (is_moving) {
        Vec mousepos = mstate.pos;
        if (mousepos.x != hold_pos.x || mousepos.y != hold_pos.y) {
            Move (mousepos - hold_pos);
            Show();
            Render(*GetRendertarget(), GetRegset());
            RenderSubWidgets(*GetRendertarget());
            hold_pos = mousepos;
        }
    }
    GetSubwidgets() -> MouseMove (mstate);
}

void Window::KeyboardPress (const KeyboardState& kstate) {
    GetSubwidgets() -> KeyboardPress (kstate);
}

void Window::KeyboardRelease (const KeyboardState& kstate) {
    GetSubwidgets() -> KeyboardRelease (kstate);
}

void Window::TimerEvent (double time) {
    GetSubwidgets() -> TimerEvent (time);
}



Background::Background (double w_, double h_):
    Widget (0, 0, w_, h_)
    {}

void Background::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawRect (GetBounds(), BG_COLOR, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 0, 255, 128));
    #endif
}

void Background::MousePress (const MouseState& mstate) {
    GetSubwidgets() -> MousePress (mstate);
}

void Background::MouseRelease (const MouseState& mstate) {
    GetSubwidgets() -> MouseRelease (mstate);
}

void Background::MouseMove (const MouseState& mstate) {
    GetSubwidgets() -> MouseMove (mstate);
}

void Background::KeyboardPress (const KeyboardState& kstate) {
    GetSubwidgets() -> KeyboardPress (kstate);
}

void Background::KeyboardRelease (const KeyboardState& kstate) {
    GetSubwidgets() -> KeyboardRelease (kstate);
}

void Background::TimerEvent (double time) {
    GetSubwidgets() -> TimerEvent (time);
}


ModalWindow::ModalWindow (double x, double y, double w, double h, EventManager* event_man_):
    Window (x, y, w, h, false),
    event_man (event_man_)
    {
        event_man -> AddObject (this);
        SetPriority (1);
        event_man -> SetMinPriorities (std::vector<Events>({MOUSE_PRESS, MOUSE_RELEASE, MOUSE_MOVE, KEYBOARD_PRESS, KEYBOARD_RELEASE}), 1);
    }

ModalWindow::~ModalWindow() {
    event_man -> ResetPriorities();
    event_man -> RemoveObject (this);
}

void window_close_btn_action (BtnArgs* btn_args) {
    ((WindowCloseBtnArgs*) btn_args) -> win -> need_to_close = true;
}

WindowCloseBtn::WindowCloseBtn (double x, double y, double w, double h, Window* win_):
    Button (x, y, w, h, window_close_btn_action, &wclose_btn_args),
    wclose_btn_args (win_)
    {}

void WindowCloseBtn::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawRect (GetBounds(), Color(255, 0, 0), to_draw);
}
