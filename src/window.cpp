#include "window.h"

Window::Window (int x, int y, size_t w, size_t h):
    Widget (x, y, w, h),
    is_moving (false)
    {}

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
    GetSubwidgets() -> MousePress (mstate);
    if (MouseOnWidget(mstate.pos)) {
        if (Rect(GetPos().x, GetPos().y, GetSize().x, 30).Contains(mstate.pos) && mstate.btn == MOUSE_LEFT) {
                is_moving = true;
                hold_pos = mstate.pos;
            }
        Show();
    }
}

void Window::MouseRelease (const MouseState& mstate) {
    if (mstate.btn == MOUSE_LEFT && is_moving) {
        is_moving = false;
        hold_pos = Vec (0, 0);
    }
    GetSubwidgets() -> MouseRelease (mstate);
}

void Window::MouseMove (const MouseState& mstate) {
    if (is_moving) {
        Vec mousepos = mstate.pos;
        if (mousepos.x != hold_pos.x || mousepos.y != hold_pos.y) {
            Move (mousepos - hold_pos);
            //regset.regions.Clear();
            Show();
            Render(*GetRendertarget(), GetRegset());
            RenderSubWidgets(*GetRendertarget());
            hold_pos = mousepos;
        }
    }
    GetSubwidgets() -> MouseMove (mstate);
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