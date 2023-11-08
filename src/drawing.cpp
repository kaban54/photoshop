#include "drawing.h"

ToolManager::ToolManager ():
    cur_tool (nullptr),
    col (Color(0, 0, 0))
    {}


inline void ToolManager::SetTool (Tool* tool) {
    cur_tool = tool;
}

inline void ToolManager::SetColor (const Color& col_) {
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



Canvas::Canvas (int x, int y, int w, int h, ToolManager* tm, FilterManager* fm):
    Widget (x, y, w, h), 
    drawing (false),
    tool_man (tm),
    filter_man (fm),
    data (w, h),
    tmp (w, h)
    {
        data.ClearScreen(Color(255, 255, 255));
        tmp.ClearScreen(Color(0, 0, 0, 0));
    }

void Canvas::MousePress (const MouseState& mstate) {
    if (MouseOnWidget(mstate.pos)) {
        Show();
        if (filter_man -> IsActive()) filter_man -> Apply (data);
        else drawing = tool_man -> PaintOnPress (&data, &tmp, MouseState(mstate.pos - GetPos(), mstate.btn));
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

inline void Brush::SetRadius (unsigned int r) {
    radius = r;
}

bool Brush::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    perm -> DrawCircle (mstate.pos, radius, col);
    last_pos = mstate.pos;
    return true;
}

bool Brush::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    perm -> DrawCircle (mstate.pos, radius, col);
    return false;
}

bool Brush::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    perm -> DrawCircle (pos, radius, col);
    perm -> DrawLine (last_pos, pos, radius * 2, col);
    last_pos = pos;
    return true;
}

inline bool Brush::Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) {
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


LineTool::LineTool (unsigned int thikness_):
    thikness (thikness_)
    {}

inline void LineTool::SetThikness (unsigned int thikness_) {
    thikness = thikness_;
}

bool LineTool::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    start_pos = mstate.pos;
    last_pos = mstate.pos;
    tmp -> DrawLine (mstate.pos, mstate.pos, thikness, col);
    return true;
}

bool LineTool::PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawLine (start_pos, mstate.pos, thikness, col);
    return false;
}

bool LineTool::PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    tmp -> DrawLine (start_pos, pos, thikness, col);
    last_pos = pos;
    return true;
}

bool LineTool::Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawLine (start_pos, last_pos, thikness, col);
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


PolyLine::PolyLine (unsigned int thikness_):
    thikness (thikness_)
    {
        start_pos = Vec(-1, -1);
    }

inline void PolyLine::SetThikness (unsigned int thikness_) {
    thikness = thikness_;
}

bool PolyLine::PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) {
    if (start_pos.x == -1 && start_pos.y == -1) {
        start_pos = mstate.pos;
        last_pos = mstate.pos;
        tmp -> DrawLine (mstate.pos, mstate.pos, thikness, col);
        if (thikness > 1) tmp -> DrawCircle (mstate.pos, thikness / 2, col);
        return true;
    }
    else {
        tmp -> ClearScreen (Color (0, 0, 0, 0));
        perm -> DrawLine (start_pos, mstate.pos, thikness, col);
        if (thikness > 1) {
            perm -> DrawCircle (mstate.pos, thikness / 2, col);
            perm -> DrawCircle (start_pos , thikness / 2, col);
        }
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
    tmp -> DrawLine (start_pos, pos, thikness, col);
    if (thikness > 1) {
        tmp -> DrawCircle (      pos, thikness / 2, col);
        tmp -> DrawCircle (start_pos, thikness / 2, col);
    }
    last_pos = pos;
    return true;
}

bool PolyLine::Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) {
    tmp -> ClearScreen (Color (0, 0, 0, 0));
    perm -> DrawLine (start_pos, last_pos, thikness, col);
    start_pos = Vec (-1, -1);
    return false;
}


void tool_btn_action (BtnArgs* tool_btn_args) {
    ToolManager* tool_man = ((ToolBtnArgs*)tool_btn_args) -> tool_man;
    Tool*        tool     = ((ToolBtnArgs*)tool_btn_args) -> tool;
    tool_man -> SetTool (tool);
}

ToolBtn::ToolBtn (double x, double y, double w, double h, const Text& txt_, ToolManager* tm, Tool* tool_):
    TxtButton (x, y, w, h, tool_btn_action, &tool_btn_args, txt_),
    tool_btn_args (tm, tool_)
    {}



void col_btn_action (BtnArgs* col_btn_args) {
    ToolManager* tool_man = ((ColorBtnArgs*)col_btn_args) -> tool_man;
    Color        color    = ((ColorBtnArgs*)col_btn_args) -> col;
    tool_man -> SetColor (color);
}

ColorBtn::ColorBtn (double x, double y, double w, double h, ToolManager* tm, const Color& col):
    Button (x, y, w, h, col_btn_action, &col_btn_args),
    col_btn_args {tm, col}
    {}

void ColorBtn::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    DrawButton (rt, GetBounds(), state, to_draw);
    rt.DrawRect (Rect (GetPos() + GetSize() / 4, GetPos() + GetSize() * 3 / 4), col_btn_args.col, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 0, 128));
    #endif
}