#include "app.h"

Gui::Gui(unsigned int w, unsigned int h, RenderTarget* rt_):
    width (w),
    height (h),
    rt (rt_)
    {
        root = new Background(w, h);
        root -> SetRenderTarget(rt);
    }

Gui::~Gui() {
    delete root;
}

Vec2 Gui::getSize() {
    return Vec2(width, height);
}

RenderTargetI* Gui::getRenderTarget(Vec2 size, Vec2 pos, Plugin *self) {
    return rt;
}

void Gui::createParamWindow(Array<const char *> param_names, Interface * self) {
    
}

WidgetI* Gui::getRoot() {
    return root;
}


App::App(unsigned int w, unsigned int h, EventManagerI* event_man, RenderTarget* rt_):
    gui (new Gui (w, h, rt_)),
    event_manager (event_man),
    tool_manager (new ToolManager),
    filter_manager (new FilterManager)
    {
        event_manager -> registerObject (gui->getRoot());
    }

App::~App() {
    delete gui;
    delete tool_manager;
    delete filter_manager;
}

void App::SetWidgets() {
    Window *mainwin = new Window (100, 100, 1920, 1080);
    tools.PushBack(new Brush(25));
    tool_manager -> setTool(tools[0]);
    tool_manager -> setColor(Color(255, 0, 128));

    VerticalMenu* tools_vm = new VerticalMenu (405, 105);
    tools_vm -> AddButton (new ToolBtn (0, 0, 200, 80, "brush", 30, tool_manager, tools[0]));

    VerticalMenu* cols_vm = new VerticalMenu (405, 185);
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(255, 0, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(0, 255, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(0, 0, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(255, 0, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(255, 255, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(0, 255, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(0, 0, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(255, 255, 255)));

    filters.PushBack (new TestFilter);
    VerticalMenu* filters_vm = new VerticalMenu (405, 265);
    filters_vm -> AddButton (new FilterBtn (0, 0, 200, 80, "test filter", 30,
                                            filter_manager, filters[0], event_manager, mainwin));

    VerticalMenu* vm = new VerticalMenu (205, 105);
    vm -> registerSubWidget (tools_vm);
    vm -> registerSubWidget (cols_vm);
    vm -> registerSubWidget (filters_vm);
    vm -> AddButton (new MenuBtn (0, 0, 200, 80,  "tools" , 30,   tools_vm));
    vm -> AddButton (new MenuBtn (0, 0, 200, 80, "colors" , 30,    cols_vm));
    vm -> AddButton (new MenuBtn (0, 0, 200, 80, "filters", 30, filters_vm));
    mainwin -> registerSubWidget (vm);
    mainwin -> registerSubWidget (new MenuBtn (205, 25, 200, 80, "edit", 30, vm));

    Window* win = new Window (200, 200, 610, 630);
    win -> registerSubWidget (new Canvas (5, 25, 600, 600, tool_manager, filter_manager));
    mainwin -> registerSubWidget (win);
    win = new Window (1000, 100, 610, 630);
    win -> registerSubWidget (new Canvas (5, 25, 600, 600, tool_manager, filter_manager));
    mainwin -> registerSubWidget (win);

    gui -> getRoot() -> registerSubWidget(mainwin);
}