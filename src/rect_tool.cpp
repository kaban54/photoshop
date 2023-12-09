#include "standart2.h"
#include <cmath>
#include <iostream>

using namespace plugin;

struct Rect {
    double x;
    double y;
    double w;
    double h;

    explicit Rect (const Vec2& p1, const Vec2& p2):
        x (p1.x < p2.x ? p1.x : p2.x),
        y (p1.y < p2.y ? p1.y : p2.y),
        w (fabs (p2.x - p1.x)),
        h (fabs (p2.y - p1.y))
        {}

    Vec2 GetPos() const {return Vec2 (x, y);}

    Vec2 GetSize() const {return Vec2 (w, h);}
};


class RectTool : public ToolI {
    Texture* icon;
    Vec2 start_pos;
    bool drawing;

    public:

    RectTool() {drawing = false;}

    virtual const Texture *getIcon() const {return icon;}

    void SetIcon(Texture *icon_) {icon = icon_;}

    virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void disable        (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;

    virtual Array<const char *> getParamNames() const override;
    virtual Array<double> getParams() const override;
    virtual void setParams(Array<double> new_params) override;
};

void RectTool::paintOnPress (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    start_pos = context.position;
    drawing = true;
}

void RectTool::paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    if (!drawing) return;
    tmp -> clear(Color(0, 0, 0, 0));
    Rect rect (start_pos, context.position);
    data -> drawRect(rect.GetPos(), rect.GetSize(), color);
    drawing = false;
}

void RectTool::paintOnMove (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    if (!drawing) return;
    tmp -> clear(Color(0, 0, 0, 0));
    Rect rect (start_pos, context.position);
    tmp -> drawRect(rect.GetPos(), rect.GetSize(), color);
}

void RectTool::disable (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    paintOnRelease(data, tmp, context, color);
}

Array<const char *> RectTool::getParamNames() const {
    return Array<const char *> (0, nullptr);
}

Array<double> RectTool::getParams() const {
    return Array<double> (0, nullptr);
}

void RectTool::setParams(Array<double> new_params) {
    assert(new_params.size == 0);
}

class RectToolPlug : public Plugin {
    RectTool* tool;
    Texture* texture;

    public:

    RectToolPlug() {
        id = (1ull << 54) + 1;
        name = "Rectangle";
        type = InterfaceType::Tool;
        tool = new RectTool;
    }

    ~RectToolPlug() {
        delete tool;
        delete texture;
    }

    virtual Interface *getInterface() const override {return tool;}

    virtual void selectPlugin() override {};

    void SetTexture (Texture* texture_) {
        texture = texture_;
        tool -> SetIcon(texture);
    }
};

extern "C" plugin::Plugin* getInstance(plugin::App *app) {
    RectToolPlug* plugin = new RectToolPlug;
    plugin -> SetTexture(app -> root -> loadTextureFromFile("rect_tool.png"));
    return plugin;
}
