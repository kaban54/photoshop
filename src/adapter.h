#ifndef ADAPTER_H
#define ADAPTER_H

#include "standart1.h"
// #include "standart2.h"
#include "app.h"

typedef pluginv1::Plugin* (*getInstance_v1_t)(pluginv1::App *app);
typedef plugin::Plugin* (*getInstance_v2_t)(plugin::App *app);

struct RTAdapter : public pluginv1::RenderTargetI {
    plugin::RenderTargetI* rt;

    explicit RTAdapter(plugin::RenderTargetI* rt_): rt(rt_) {}
    explicit RTAdapter() = default;

    virtual void setPixel(pluginv1::Vec2 pos, pluginv1::Color color) override {
        rt -> setPixel(plugin::Vec2(pos.x, pos.y), plugin::Color(color.r, color.g, color.b, color.a));
    }

    virtual void drawLine(pluginv1::Vec2 point1, pluginv1::Vec2 point2, pluginv1::Color color) override {
        rt -> drawLine(plugin::Vec2(point1.x, point2.y),
                       plugin::Vec2(point1.x, point2.y),
                       plugin::Color(color.r, color.g, color.b, color.a));
    }

    virtual void drawRect(pluginv1::Vec2 pos, pluginv1::Vec2 size, pluginv1::Color color) override {
        rt -> drawRect(plugin::Vec2(pos.x, pos.y),
                       plugin::Vec2(size.x, size.y),
                       plugin::Color(color.r, color.g, color.b, color.a));
    }

    virtual void drawEllipse(pluginv1::Vec2 pos, pluginv1::Vec2 size, pluginv1::Color color) override {
        rt -> drawEllipse(plugin::Vec2(pos.x, pos.y),
                          plugin::Vec2(size.x, size.y),
                          plugin::Color(color.r, color.g, color.b, color.a));
    }

    virtual void drawTexture(pluginv1::Vec2 pos, pluginv1::Vec2 size, const pluginv1::Texture *texture) override {
        plugin::Texture tv2 (texture -> width, texture -> height, (plugin::Color*)(texture -> pixels));
        rt -> drawTexture(plugin::Vec2(pos.x, pos.y),
                          plugin::Vec2(size.x, size.y),
                          &tv2);
    }

    virtual void drawText(pluginv1::Vec2 pos, const char *content, uint16_t char_size, pluginv1::Color color) override {
        rt -> drawText(plugin::Vec2(pos.x, pos.y), content, char_size,
                       plugin::Color(color.r, color.g, color.b, color.a));
    }

    virtual pluginv1::Texture *getTexture() override {
        plugin::Texture* tv2 = rt -> getTexture();
        pluginv1::Texture* tv1 = new pluginv1::Texture(tv2 -> width, tv2 -> height, (pluginv1::Color*)(tv2 -> pixels));
        delete tv2;
        return tv1;
    }

    virtual void display() override {rt -> display();}

    virtual void clear() override {rt -> clear();}
};

class ToolAdapter : public plugin::ToolI {
    pluginv1::ToolI* tool;
    plugin::Texture* icon;
    RTAdapter rt1;
    RTAdapter rt2;

    public:
    explicit ToolAdapter(pluginv1::ToolI* tool_): tool(tool_) {
        const pluginv1::Texture* iconv1 = tool -> getIcon();
        if (iconv1 != nullptr) {
            icon = new plugin::Texture(iconv1 -> width, iconv1 -> height, (plugin::Color*)(iconv1 -> pixels));
        }
    }

    ~ToolAdapter() {
        if (icon != nullptr) delete icon;
    }

    virtual const plugin::Texture *getIcon() const override {return icon;}

    virtual void paintOnPress  (plugin::RenderTargetI *data, plugin::RenderTargetI *tmp,
                                plugin::MouseContext context, plugin::Color color) override {
        rt1.rt = data;
        rt2.rt = tmp;
        pluginv1::MouseContext contextv1 (pluginv1::Vec2(context.position.x, context.position.y), (int)context.button);
        tool -> paintOnPress(&rt1, &rt2, contextv1, pluginv1::Color(color.r, color.g, color.b, color.a));
    }

    virtual void paintOnRelease(plugin::RenderTargetI *data, plugin::RenderTargetI *tmp,
                                plugin::MouseContext context, plugin::Color color) override {
        rt1.rt = data;
        rt2.rt = tmp;
        pluginv1::MouseContext contextv1 (pluginv1::Vec2(context.position.x, context.position.y), (int)context.button);
        tool -> paintOnRelease(&rt1, &rt2, contextv1, pluginv1::Color(color.r, color.g, color.b, color.a));
    }

    virtual void paintOnMove   (plugin::RenderTargetI *data, plugin::RenderTargetI *tmp,
                                plugin::MouseContext context, plugin::Color color) override {
        rt1.rt = data;
        rt2.rt = tmp;
        pluginv1::MouseContext contextv1 (pluginv1::Vec2(context.position.x, context.position.y), (int)context.button);
        tool -> paintOnMove(&rt1, &rt2, contextv1, pluginv1::Color(color.r, color.g, color.b, color.a));
    }

    virtual void disable(plugin::RenderTargetI *data, plugin::RenderTargetI *tmp,
                         plugin::MouseContext context, plugin::Color color) override {}

    virtual plugin::Array<const char *> getParamNames() const override {
        pluginv1::Array<const char *> arrv1 = tool -> getParamNames();
        return plugin::Array<const char*> (arrv1.size, arrv1.data);
    }
        
    virtual plugin::Array<double> getParams() const override {
        pluginv1::Array<double> arrv1 = tool -> getParams();
        return plugin::Array<double> (arrv1.size, arrv1.data);
    }

    virtual void setParams(plugin::Array<double> params) override {
        pluginv1::Array<double> arrv1(params.size, params.data);
        tool -> setParams(arrv1);
    }
};

class TMAdapter : public pluginv1::ToolManagerI {
    ToolManager* tm;
    ToolAdapter* tool_adapter;

    public:
    explicit TMAdapter(ToolManager* tm_): tm(tm_) {}

    ~TMAdapter() {
        if (tool_adapter != nullptr) delete tool_adapter;
    }

    virtual void setColor(pluginv1::Color color) override {
        tm -> setColor(plugin::Color(color.r, color.g, color.b, color.a));
    }

    virtual void setTool(pluginv1::ToolI *tool) override {
        if (tool_adapter != nullptr) delete tool_adapter;
        tool_adapter = new ToolAdapter(tool);
        tm -> setTool(tool_adapter);
    }

    virtual pluginv1::ToolI *getTool() override {
        std::cerr << "Error: Unable to convert Toolv2 to Toolv1\n";
        return nullptr;
    }
    virtual pluginv1::Color getColor() override {
        plugin::Color col = tm -> getColor();
        return pluginv1::Color(col.r, col.g, col.b, col.a);
    }

    virtual void paintOnMove   (pluginv1::RenderTargetI *data, pluginv1::RenderTargetI *tmp, pluginv1::MouseContext context) override {std::cerr << "Error: not ready yet :(\n";}
    virtual void paintOnPress  (pluginv1::RenderTargetI *data, pluginv1::RenderTargetI *tmp, pluginv1::MouseContext context) override {std::cerr << "Error: not ready yet :(\n";}
    virtual void paintOnRelease(pluginv1::RenderTargetI *data, pluginv1::RenderTargetI *tmp, pluginv1::MouseContext context) override {std::cerr << "Error: not ready yet :(\n";}
    virtual void disableTool   (pluginv1::RenderTargetI *data, pluginv1::RenderTargetI *tmp, pluginv1::MouseContext context) override {std::cerr << "Error: not ready yet :(\n";}
};


plugin::Plugin* GetPlugV2(getInstance_v1_t get_inst_v1, MyApp* myapp);

#endif