#ifndef DRAWING_H
#define DRAWING_H

#include "buttons.h"


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
    RenderTarget data;
    RenderTarget tmp;

    public:

    explicit Canvas (int x, int y, int w, int h, ToolManager* tm);

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
};


class Brush : public Tool {
    unsigned int radius;

    public:

    explicit Brush (unsigned int r);

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
    public:

    explicit LineTool() {}

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
    public:

    explicit PolyLine();

    virtual bool PaintOnPress (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnRelease (RenderTarget* perm, RenderTarget *tmp, const MouseState& mstate, const Color& col) override;

    virtual bool PaintOnMove (RenderTarget* perm, RenderTarget *tmp, const Vec& pos, const Color& col) override;

    virtual bool Deactivate (RenderTarget* perm, RenderTarget *tmp, const Color& col) override;
};


class ToolBtn : public TxtButton {
    ToolManager* tool_man;
    Tool* tool;

    public:

    ToolBtn (double x, double y, size_t w, size_t h, const Texture* textures_, const Text& txt_, ToolManager* tm, Tool* tool_);

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

};

class ColorBtn : public ImgButton {
    ToolManager* tool_man;
    Color color;

    public:

    ColorBtn (double x, double y, size_t w, size_t h, const Texture* textures_, ToolManager* tm, const Color& col);

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;
};

#endif