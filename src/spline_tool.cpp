#include "standart2.h"
#include "catmullrom.h"
#include "mylist.h"
#include <iostream>

using namespace plugin;

class SplineTool : public ToolI {
    Texture* icon;
    Vec2 points[4];
    size_t num_of_points;
    double thikness;
    bool drawing;

    public:

    SplineTool():
        num_of_points (0),
        thikness (10),
        drawing (false) {}

    virtual const Texture *getIcon() const {return icon;}

    void SetIcon(Texture *icon_) {icon = icon_;}

    virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override {}
    virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override {}
    virtual void disable        (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;

    virtual Array<const char *> getParamNames() const override;
    virtual Array<double> getParams() const override;
    virtual void setParams(Array<double> new_params) override;
};

void SplineTool::paintOnPress(RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    if (context.button == MouseButton::Left) {
        if (num_of_points > 0 && context.position == points[num_of_points - 1]) return;
        if (num_of_points < 4) {
            drawing = true;
            points[num_of_points++] = context.position;
        }
        else {
            points[0] = points[1];
            points[1] = points[2];
            points[2] = points[3];
            points[3] = context.position;
        }

        switch (num_of_points) {
        case 1:
            data -> drawEllipse(context.position - Vec2(thikness, thikness) / 2, Vec2(thikness, thikness), color);
            break;
        case 2:
            tmp -> clear();
            DrawSpline(tmp, points[0], points[1], color, thikness);
            break;
        case 3:
            tmp -> clear();
            DrawSpline(tmp , points[0], points[1], points[2], color, thikness);
            DrawSpline(data, points[2], points[1], points[0], color, thikness);
            break;
        case 4:
            tmp -> clear();
            DrawSpline(data, points[0], points[1], points[2], points[3], color, thikness);
            DrawSpline(tmp ,            points[1], points[2], points[3], color, thikness);
            break;
        default:
            break;
        }
    }
    else if (context.button == MouseButton::Right && drawing) {
        disable(data, tmp, context, color);
    }
}

void SplineTool::disable(RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) {
    switch (num_of_points) {
    case 2:
        tmp -> clear();
        DrawSpline(data, points[0], points[1], color, thikness);
        break;
    case 3:
        tmp -> clear();
        DrawSpline(data, points[0], points[1], points[2], color, thikness);
        break;
    case 4:
        tmp -> clear();
        DrawSpline(data, points[1], points[2], points[3], color, thikness);
        break;
    default:
        break;
    }

    num_of_points = 0;
    drawing = false;
}

Array<const char *> SplineTool::getParamNames() const {
    return Array<const char *> (0, nullptr);
}

Array<double> SplineTool::getParams() const {
    return Array<double> (0, nullptr);
}

void SplineTool::setParams(Array<double> new_params) {
    assert(new_params.size == 0);
}

class SplineToolPlug : public Plugin {
    SplineTool* tool;
    Texture* texture;

    public:

    SplineToolPlug() {
        id = (1ull << 54) + 's' + 'p' + 'l' + 'i' + 'n' + 'e';
        name = "Spline";
        type = InterfaceType::Tool;
        tool = new SplineTool;
    }

    ~SplineToolPlug() {
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
    SplineToolPlug* plugin = new SplineToolPlug;
    plugin -> SetTexture(app -> root -> loadTextureFromFile("spline_tool.png"));
    return plugin;
}
