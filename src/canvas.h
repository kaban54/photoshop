#ifndef CANVAS_H
#define CANVAS_H

#include "widget.h"
#include "tool.h"
#include "window.h"

class CanvasWindow;

class Canvas : public Widget {
    bool drawing;
    ToolManager* tool_man;
    CanvasWindow* win;
    RenderTarget data;
    RenderTarget tmp;

    public:

    explicit Canvas (double x, double y, double w, double h,
                     ToolManager* tm, CanvasWindow* win = nullptr);

    RenderTarget* GetData() {return &data;}

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override;
    virtual bool onMouseMove    (MouseContext context) override;
    virtual bool onKeyboardPress   (KeyboardContext context) override {return false;}
    virtual bool onKeyboardRelease (KeyboardContext context) override {return false;}
    virtual bool onClock (uint64_t delta) override;

    virtual void RenderInRegset(RenderTarget& rt, const RegionSet* to_draw) override;
};

class ImageManager {
    MyList<CanvasWindow*> windows;

    public:

    explicit ImageManager();

    void AddWindow(CanvasWindow* win);

    void RemoveWindow(CanvasWindow* win);

    CanvasWindow* GetActive() const;

    void SetActive(CanvasWindow* win);

    size_t GetSize() const {return windows.GetSize();}
};

class CanvasWindow : public Window {
    ImageManager* image_man;
    Canvas* canvas;

    public:

    explicit CanvasWindow (double x, double y, double w, double h,
                           ImageManager* image_man_, ToolManager* tool_man);

    Canvas* GetCanvas() {return canvas;}

    void SetActive();

    virtual void Close() override;
};

#endif