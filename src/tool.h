#ifndef TOOL_H
#define TOOL_H

#include "interface.h"
#include "rendertarget.h"
#include "events.h"

namespace plugin {
    struct ToolI: public Interface {
        virtual const Texture *getIcon() = 0;

        virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void disable        (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
    };

    struct ToolManagerI {
        virtual void setColor(Color color) = 0;
        virtual void setTool(ToolI *tool) = 0;

        virtual ToolI *getTool() = 0;
        virtual Color  getColor() = 0;

        virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void disableTool    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
    };
}

using namespace plugin;

struct Tool : public ToolI {
    Vec2 start_pos;
    Vec2 last_pos;
    Texture* icon;

    explicit Tool(Texture* icon_ = nullptr);

    void SetIcon(Texture* icon_);

    virtual const Texture *getIcon() override;
};

class ToolManager : public ToolManagerI{
    ToolI* tool;
    Color col;

    public:

    explicit ToolManager() {}

    virtual void setColor(Color color) override;
    virtual void setTool(ToolI *tool_) override;

    virtual ToolI *getTool() override;
    virtual Color  getColor() override;

    virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) override;
    virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) override;
    virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) override;
    virtual void disableTool    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context) override;
};

class Brush : public Tool {
    unsigned int radius;

    public:

    explicit Brush (unsigned int r = 1);

    virtual void paintOnPress   (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void paintOnRelease (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void paintOnMove    (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;
    virtual void disable        (RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) override;

    virtual Array<const char *> getParamNames() override;
    virtual Array<double> getParams() override;
    virtual void setParams(Array<double> params) override;
};

#endif