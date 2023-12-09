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


WidgetI* Gui::getRoot() const {
    return root;
}


void Gui::createWidgetI(PluginWidgetI* widget) {
    // widget -> host = new Widget;
}

Plugin* Gui::queryPlugin(uint64_t id) {
    return nullptr;
}

Texture* Gui::loadTextureFromFile(const char *filename) {
    sf::Texture sft;
    sft.loadFromFile(filename);
    sf::Image sfimg = sft.copyToImage();
    return new Texture (sfimg.getSize().x, sfimg.getSize().y, (Color*)sfimg.getPixelsPtr());
}

MyApp::MyApp(unsigned int w, unsigned int h, EventManagerI* event_man, RenderTarget* rt_) {
    gui = new Gui (w, h, rt_);
    event_manager = event_man;
    tool_manager = new ToolManager;
    filter_manager = new FilterManager;
    event_manager -> registerObject (dynamic_cast<EventProcessableI*>(gui -> getRoot()));
}

MyApp::~MyApp() {
    delete gui;
    delete tool_manager;
    delete filter_manager;

    
    for (size_t i = 0; i < tools   .GetSize(); i++) delete tools   [i];
    for (size_t i = 0; i < filters .GetSize(); i++) delete filters [i];
    for (size_t i = 0; i < textures.GetSize(); i++) delete textures[i];
    // for (size_t i = 0; i < plugins.GetSize(); i++) delete plugins[i];
}

void MyApp::SetupWidgets() {
    Window *mainwin = new Window (50, 50, 1920, 1080);
    mainwin->SetName ("Test");

    Brush* brush = new Brush(25);
    textures.PushBack(gui -> loadTextureFromFile("textures/brush_icon.png"));
    brush -> SetIcon(textures.Back());

    tools.PushBack(brush);
    tool_manager -> setTool(tools[0]);
    tool_manager -> setColor(Color(255, 0, 128));

    tools_vm = new VerticalMenu (405, 115);

    tools_tcm = new TwoColMenu (5, 140, 70, 70);
    tools_tcm -> setAvailable (true);
    mainwin -> registerSubWidget (tools_tcm);
    tools_vm -> AddButton (new ToolTxtBtn (0, 0, 200, 80, "brush", 30, tool_manager, tools[0]));
    tools_tcm -> AddButton (new ToolImgBtn(0, 0, 70, 70, tools[0] -> getIcon(), tool_manager, tools[0]));
    // mainwin -> registerSubWidget (new ToolImgBtn (5, 140, 70, 70, tools[0] -> getIcon(), tool_manager, tools[0]));
    // mainwin -> registerSubWidget (new ToolImgBtn (75, 140, 70, 70, tools[0] -> getIcon(), tool_manager, tools[0]));

    VerticalMenu* cols_vm = new VerticalMenu (405, 195);
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(255, 0, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(0, 255, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(0, 0, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(255, 0, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(255, 255, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(0, 255, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(0, 0, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, tool_manager, Color(255, 255, 255)));

    TwoColMenu* cols_tcm = new TwoColMenu (5, 610, 70, 70);
    cols_tcm -> setAvailable (true);
    mainwin -> registerSubWidget (cols_tcm);
    cols_tcm -> AddButton (new ColorBtn (0, 0, 70, 70, tool_manager, Color(255, 0, 0)));
    cols_tcm -> AddButton (new ColorBtn (0, 0, 70, 70, tool_manager, Color(0, 255, 0)));
    cols_tcm -> AddButton (new ColorBtn (0, 0, 70, 70, tool_manager, Color(0, 0, 255)));
    cols_tcm -> AddButton (new ColorBtn (0, 0, 70, 70, tool_manager, Color(255, 0, 255)));
    cols_tcm -> AddButton (new ColorBtn (0, 0, 70, 70, tool_manager, Color(255, 255, 0)));
    cols_tcm -> AddButton (new ColorBtn (0, 0, 70, 70, tool_manager, Color(0, 255, 255)));
    cols_tcm -> AddButton (new ColorBtn (0, 0, 70, 70, tool_manager, Color(0, 0, 0)));
    cols_tcm -> AddButton (new ColorBtn (0, 0, 70, 70, tool_manager, Color(255, 255, 255)));

    filters.PushBack (new InvFilter);
    filters.PushBack (new ClearFilter);
    filters_vm = new VerticalMenu (405, 275);
    filters_vm -> AddButton (new FilterBtn (0, 0, 300, 80, "inversion", 30,
                                            filter_manager, filters[0], event_manager, mainwin));
    filters_vm -> AddButton (new FilterBtn (0, 0, 300, 80, "clear", 30,
                                            filter_manager, filters[1], event_manager, mainwin));
    VerticalMenu* vm = new VerticalMenu (205, 115);
    vm -> registerSubWidget (tools_vm);
    vm -> registerSubWidget (cols_vm);
    vm -> registerSubWidget (filters_vm);
    vm -> AddButton (new MenuBtn (0, 0, 200, 80,  "tools" , 30,   tools_vm));
    vm -> AddButton (new MenuBtn (0, 0, 200, 80, "colors" , 30,    cols_vm));
    vm -> AddButton (new MenuBtn (0, 0, 200, 80, "filters", 30, filters_vm));
    mainwin -> registerSubWidget (vm);
    mainwin -> registerSubWidget (new MenuBtn (205, 35, 200, 80, "edit", 30, vm));
    Window* win = new Window (150, 140, 870, 930);
    win -> registerSubWidget (new Canvas (5, 35, 860, 890, tool_manager, filter_manager));
    mainwin -> registerSubWidget (win);
    win = new Window (1030, 140, 870, 930);
    win -> registerSubWidget (new Canvas (5, 35, 860, 890, tool_manager, filter_manager));
    mainwin -> registerSubWidget (win);
    gui -> getRoot() -> registerSubWidget(mainwin);
}

void MyApp::AddPlugin(Plugin* plug) {
    plugins.PushBack(plug);
    if (plug -> type == InterfaceType::Tool) {
        ToolI* tool = dynamic_cast<ToolI*>(plug -> getInterface());
        tools_vm -> AddButton (new ToolTxtBtn (0, 0, 200, 80, plug -> name, 30, tool_manager, tool));
    }
    else if (plug -> type == InterfaceType::Filter) {
        FilterI* filter = dynamic_cast<FilterI*>(plug -> getInterface());
        filters_vm -> AddButton (new FilterBtn (0, 0, 300, 80, plug -> name, 30, filter_manager,
                                                filter, event_manager, gui -> getRoot()));
    }
}
