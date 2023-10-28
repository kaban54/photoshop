#include "drawing.h"

ToolManager::ToolManager ():
    cur_tool (nullptr),
    col (Color(0, 0, 0))
    {}


void ToolManager::SetTool (Tool* tool) {
    cur_tool = tool;
}

void ToolManager::SetColor (const Color& col_) {
    col = col_;
}

bool ToolManager::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate) {
    return cur_tool -> PaintOnPress (perm, tmp, mstate, col);
}

bool ToolManager::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate) {
    return cur_tool -> PaintOnRelease (perm, tmp, mstate, col);
}

bool ToolManager::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos) {
    return cur_tool -> PaintOnMove (perm, tmp, pos, col);
}

bool ToolManager::Deactivate (RenderTarget* perm, RenderTarget *tmp) {
    return cur_tool -> Deactivate (perm, tmp, col);
}



Canvas::Canvas (int x, int y, int w, int h, ToolManager* tm):
    Widget (x, y, w, h), 
    drawing (false),
    tool_man (tm),
    data (w, h),
    tmp (w, h)
    {
        data.ClearScreen(Color(255, 255, 255));
        tmp.ClearScreen(Color(0, 0, 0, 0));
    }

void Canvas::MousePress (const MouseState& mstate) {
    if (MouseOnWidget(mstate.pos)) {
        Show();
        drawing = tool_man -> PaintOnPress (&data, &tmp, MouseState(mstate.pos - GetPos(), mstate.btn));
        Render (*GetRendertarget(), GetRegset());
    }
}

void Canvas::MouseRelease (const MouseState& mstate) {
    if (drawing) {
        drawing = tool_man -> PaintOnRelease (&data, &tmp, MouseState(mstate.pos - GetPos(), mstate.btn));
        Render (*GetRendertarget(), GetRegset());
    }
}

void Canvas::MouseMove (const MouseState& mstate) {
    if (drawing) {
        if (MouseOnWidget (mstate.pos)) drawing = tool_man -> PaintOnMove (&data, &tmp, mstate.pos - GetPos());
        else                            drawing = tool_man -> Deactivate (&data, &tmp);
        Render (*GetRendertarget(), GetRegset());
    }
}

void Canvas::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawRenderTarget(data, GetPos(), to_draw);
    rt.DrawRenderTarget(tmp , GetPos(), to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 255, 128));
    #endif
}


Brush::Brush (unsigned int r):
    radius (r)
    {}

void Brush::SetRadius (unsigned int r) {
    radius = r;
}

bool Brush::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    perm -> DrawCircle (mstate.pos, radius, col);
    return true;
}

bool Brush::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    perm -> DrawCircle (mstate.pos, radius, col);
    return false;
}

bool Brush::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    perm -> DrawCircle (pos, radius, col);
    return true;
}

bool Brush::Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) {
    return false;
}


bool RectTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    start_pos = mstate.pos;
    last_pos = mstate.pos;
    tmp -> DrawRect (Rect(mstate.pos, mstate.pos), col);
    return true;
}

bool RectTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawRect (Rect(start_pos, mstate.pos), col);
    return false;
}

bool RectTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawRect (Rect(start_pos, pos), col);
    last_pos = pos;
    return true;
}

bool RectTool::Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawRect (Rect(start_pos, last_pos), col);
    return false;
}


bool LineTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    start_pos = mstate.pos;
    last_pos = mstate.pos;
    tmp -> DrawLine (mstate.pos, mstate.pos, col);
    return true;
}

bool LineTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawLine (start_pos, mstate.pos, col);
    return false;
}

bool LineTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawLine (start_pos, pos, col);
    last_pos = pos;
    return true;
}

bool LineTool::Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawLine (start_pos, last_pos, col);
    return false;
}


bool EllipseTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    start_pos = mstate.pos;
    last_pos = mstate.pos;
    tmp -> DrawEllipse (Rect(mstate.pos, mstate.pos), col);
    return true;
}

bool EllipseTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawEllipse (Rect(start_pos, mstate.pos), col);
    return false;
}

bool EllipseTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawEllipse (Rect(start_pos, pos), col);
    last_pos = pos;
    return true;
}

bool EllipseTool::Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawEllipse (Rect(start_pos, last_pos), col);
    return false;
}


PolyLine::PolyLine () {
    start_pos = Vec(-1, -1);
}

bool PolyLine::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    if (start_pos.x == -1 && start_pos.y == -1) {
        start_pos = mstate.pos;
        last_pos = mstate.pos;
        tmp -> DrawLine (mstate.pos, mstate.pos, col);
        return true;
    }
    else {
        tmp -> ClearScreen (Color (0, 0, 0, 0));
        perm -> DrawLine (start_pos, mstate.pos, col);
        if (mstate.btn == MOUSE_RIGHT) {
            start_pos = Vec (-1, -1);
            last_pos = start_pos;
            return false;
        }
        else {
            start_pos = mstate.pos;
            last_pos = mstate.pos;
            return true;
        }
    }
}

bool PolyLine::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    return true;
}

bool PolyLine::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawLine (start_pos, pos, col);
    last_pos = pos;
    return true;
}

bool PolyLine::Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawLine (start_pos, last_pos, col);
    start_pos = Vec (-1, -1);
    return false;
}


ToolBtn::ToolBtn (double x, double y, size_t w, size_t h, const Text& txt_, ToolManager* tm, Tool* tool_):
    TxtButton (x, y, w, h, txt_),
    tool_man (tm),
    tool (tool_)
    {}

void ToolBtn::MousePress (const MouseState& mstate) {
    if (MouseOnWidget(mstate.pos) && state != BTN_PRESSED) {
        tool_man -> SetTool (tool);
        state = BTN_PRESSED;
        Render (*GetRendertarget(), GetRegset());
    }
}

void ToolBtn::MouseRelease (const MouseState& mstate) {
    if (state == BTN_PRESSED) {
        state = BTN_NORMAL;
        Render (*GetRendertarget(), GetRegset());
    }
}

ColorBtn::ColorBtn (double x, double y, size_t w, size_t h, ToolManager* tm, const Color& col):
    Button (x, y, w, h),
    tool_man (tm),
    color (col)
    {}

void ColorBtn::MousePress (const MouseState& mstate) {
    if (MouseOnWidget(mstate.pos)) {
        tool_man -> SetColor (color);
        state = BTN_PRESSED;
        Render (*GetRendertarget(), GetRegset());
    }
}

void ColorBtn::MouseRelease (const MouseState& mstate) {
    if (state == BTN_PRESSED) {
        state = BTN_NORMAL;
        Render (*GetRendertarget(), GetRegset());
    }
}

void ColorBtn::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    DrawButton (rt, GetBounds(), state, to_draw);
    rt.DrawRect (Rect (GetPos() + GetSize() / 4, GetPos() + GetSize() * 3 / 4), color, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 0, 128));
    #endif
}