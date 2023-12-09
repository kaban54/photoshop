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

        virtual Interface *getInterface() const = 0;

        // плагин выбрали (недо apply)
        virtual void selectPlugin() = 0;

        virtual ~Plugin() = default;
    };

    struct GuiI {
        virtual WidgetI* getRoot() const = 0;
        virtual void createWidgetI(PluginWidgetI* widget) = 0;
        virtual Plugin *queryPlugin(uint64_t id) = 0;
        virtual Texture *loadTextureFromFile(const char *filename) = 0;
        virtual ~GuiI() = default;
    };

    struct App {
        GuiI *gui;
        EventManagerI *event_manager;
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

    virtual WidgetI* getRoot() const override;
    virtual void createWidgetI(PluginWidgetI* widget) override;
    virtual Plugin *queryPlugin(uint64_t id) override;
    virtual Texture *loadTextureFromFile(const char *filename) override;
};

class MyApp : public App {
    ToolManager* tool_manager;
    FilterManager* filter_manager;
    ImageManager* image_manager;

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


extern "C" plugin::Plugin* getInstance(plugin::App *app);

#endif