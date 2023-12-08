#ifndef APP_H
#define APP_H

#include <cinttypes>
#include "widget.h"
#include "tool.h"
#include "filter.h"
#include "menu.h"
#include "canvas.h"


namespace plugin {
    struct Plugin {
        /* где-то тут лежит App*, но это дело автора плагина */
        uint64_t id;
        const char *name;
        InterfaceType type;

        virtual Interface *getInterface() = 0;
        virtual ~Plugin() = default;
    };

    struct GuiI {
        virtual Vec2 getSize() = 0;
        virtual RenderTargetI* getRenderTarget(Vec2 size, Vec2 pos, Plugin *self) = 0;
        virtual void createParamWindow(Array<const char *> param_names, Interface* self) = 0;
        virtual WidgetI* getRoot() = 0;
    };

    struct App {
        GuiI *gui;
        EventManagerI *event_manager; 
        ToolManagerI *tool_manager;
        FilterManagerI *filter_manager;
    };
}

using namespace plugin;

struct Gui : public GuiI {
    const unsigned int width;
    const unsigned int height;
    RenderTarget* rt;
    Background *root;

    explicit Gui(unsigned int w, unsigned int h, RenderTarget* rt_);

    ~Gui();

    virtual Vec2 getSize() override;

    virtual RenderTargetI* getRenderTarget(Vec2 size, Vec2 pos, Plugin *self) override;

    virtual void createParamWindow(Array<const char *> param_names, Interface * self) override;

    virtual WidgetI* getRoot() override;
};

class MyApp : public App {
    MyVector<ToolI*> tools;
    MyVector<FilterI*> filters;
    MyVector<Plugin*> plugins;
    MyVector<Texture*> textures;

    VerticalMenu* tools_vm;
    VerticalMenu* filters_vm;
    TwoColMenu* tools_tcm;

    public:

    explicit MyApp (unsigned int w, unsigned int h, EventManagerI* event_man, RenderTarget* rt_);

    ~MyApp();

    void SetupWidgets();

    void AddPlugin(Plugin* plug);
};


plugin::Texture* LoadFromFile(const char* filename);

extern "C" plugin::Plugin* getInstance(plugin::App *app);

#endif