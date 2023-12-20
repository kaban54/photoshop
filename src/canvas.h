#ifndef CANVAS_H
#define CANVAS_H

#include "widget.h"
#include "tool.h"
#include "window.h"
#include "menu.h"

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
    VerticalMenu* window_menu;

    public:

    explicit ImageManager();

    void SetWindowMenu(VerticalMenu* wm);    

    void AddWindow(CanvasWindow* win);

    void RemoveWindow(CanvasWindow* win);

    CanvasWindow* GetActive() const;

    void SetActive(CanvasWindow* win);

    size_t GetSize() const {return windows.GetSize();}
};

class CanvasWindow : public Window {
    ImageManager* image_man;
    Canvas* canvas;
    Button* showbtn;

    public:

    explicit CanvasWindow (double x, double y, double w, double h,
                           ImageManager* image_man_, ToolManager* tool_man);

    Canvas* GetCanvas() {return canvas;}

    void    SetShowBtn(Button* btn_) {showbtn = btn_;}
    Button* GetShowBtn() {return showbtn;}

    void SetActive();

    virtual void Close() override;
};


struct WindowShowBtnArgs : public BtnArgs {
    Window* win;
    Button* btn;
    explicit WindowShowBtnArgs (Window* win_, Button* btn_):
        win (win_),
        btn (btn_) {}
};

class WindowShowBtn : public TxtButton {
    WindowShowBtnArgs wshow_btn_args;

    public:

    explicit WindowShowBtn (double x, double y, double w, double h, Window* win_);
};

#endif