#ifndef DRAWING_H
#define DRAWING_H

#include "buttons.h"
#include "filter.h"

class Tool {
    public:
    Vec start_pos;
    Vec last_pos;

    explicit Tool() {};

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) = 0;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) = 0;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) = 0;

    virtual bool Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) = 0;
};


class ToolManager {
    Tool* cur_tool;
    Color col;

    public:

    explicit ToolManager();

    void SetTool (Tool* tool);

    void SetColor (const Color& col_);

    bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate);

    bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate);

    bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos);

    bool Deactivate (RenderTarget* perm, RenderTarget *tmp);
};


class Canvas : public Widget {
    bool drawing;
    ToolManager* tool_man;
    FilterManager* filter_man;
    RenderTarget data;
    RenderTarget tmp;

    public:

    explicit Canvas (int x, int y, int w, int h, ToolManager* tm, FilterManager* fm);

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
};


class Brush : public Tool {
    unsigned int radius;

    public:

    explicit Brush (unsigned int r = 1);

    void SetRadius (unsigned int r);

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual bool Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) override;
};

class RectTool : public Tool {
    public:

    explicit RectTool() {}

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual bool Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) override;
};

class LineTool : public Tool {
    unsigned int thikness;

    public:

    explicit LineTool (unsigned int thikness_ = 1);

    void SetThikness (unsigned int thikness_);

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual bool Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) override;
};

class EllipseTool : public Tool {
    public:

    explicit EllipseTool() {}

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual bool Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) override;
};

class PolyLine : public Tool {
    unsigned int thikness;

    public:

    explicit PolyLine (unsigned int thikness_ = 1);

    void SetThikness (unsigned int thikness_);

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual bool Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) override;
};


struct ToolBtnArgs : public BtnArgs {
    ToolManager* tool_man;
    Tool* tool;
    explicit ToolBtnArgs (ToolManager* tool_man_, Tool* tool_):
        tool_man (tool_man_),
        tool (tool_) {}
};

void tool_btn_action (BtnArgs* tool_btn_args);

class ToolBtn : public TxtButton {
    ToolBtnArgs tool_btn_args;

    public:

    explicit ToolBtn (double x, double y, double w, double h, const Text& txt_, ToolManager* tm, Tool* tool_);
};


struct ColorBtnArgs : public BtnArgs {
    ToolManager* tool_man;
    Color col;
    explicit ColorBtnArgs (ToolManager* tool_man_, Color col_):
        tool_man (tool_man_),
        col (col_) {}
};

void col_btn_action (BtnArgs* col_btn_args);

class ColorBtn : public Button {
    ColorBtnArgs col_btn_args;

    public:

    explicit ColorBtn (double x, double y, double w, double h, ToolManager* tm, const Color& col);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
};

#endif