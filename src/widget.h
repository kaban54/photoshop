#ifndef WIDGET_H
#define WIDGET_H

#include "vec.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rendertarget.h"
#include "region.h"
#include "color.h"

const size_t BASE_WIDGETMAN_CAP = 4;
const Color WINDOW_BG_COLOR = Color (128, 128, 128);

enum MouseButtons {
    MOUSE_LEFT = 0,
    MOUSE_RIGHT = 1
};




class Renderable {
    public:
    virtual void Render (RenderTarget& screen, const RegionSet& to_draw) const = 0;
};

class Widget;

class WidgetManager {
    Widget** widgets;
    size_t size;
    size_t capacity;
    
    public:
    
    WidgetManager (size_t cap = BASE_WIDGETMAN_CAP);

    ~WidgetManager();
    
    void AddWidget (Widget* widget);

    size_t GetSize() const;

    Widget* operator[] (size_t index);

    void Render (RenderTarget& screen) const;

    void Move (const Vec& vec);

    void MousePress (const Vec& mousepos, MouseButtons mousebtn);

    void MouseRelease (const Vec& mousepos, MouseButtons mousebtn);

    void MouseMove (const Vec& mousepos);
};

class Widget : public Renderable {
    WidgetManager subwidgets;
    
    public:
    Vec pos;
    Vec size;
    bool visible;
    RegionSet regset;
    Widget* parent;
    RenderTarget* rt;

    explicit Widget();

    explicit Widget (int x, int y, int w, int h, size_t subw_cap = BASE_WIDGETMAN_CAP);
    
    void SetRenderTarget (RenderTarget *rt_);

    void AddSubWidget (Widget* wid);

    void RenderSubWidgets (RenderTarget& screen) const;

    void Move (const Vec& vec);

    void UpdateRegSet (const Rect& movingwindow, Widget* no_update = nullptr);

    void Show() {UpdateRegSet(Rect(Vec(-1, -1), Vec(0, 0)));}

    void MousePress (const Vec& mousepos, MouseButtons mousebtn);

    void MouseRelease (const Vec& mousepos, MouseButtons mousebtn);

    void MouseMove (const Vec& mousepos);

    virtual void MousePressAction (const Vec& mousepos, MouseButtons mousebtn) = 0;

    virtual void MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) = 0;

    virtual void MouseMoveAction (const Vec& mousepos) = 0;

    virtual bool MouseOnWidget (const Vec& mousepos) = 0;

};


class Window : public Widget {
    bool is_moving;
    Vec hold_pos;

    public:

    explicit Window (size_t w, size_t h);

    explicit Window (int x, int y, size_t w, size_t h);

    virtual void Render (RenderTarget& screen, const RegionSet& to_draw) const override;

    virtual void MousePressAction (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) override;

    virtual void MouseMoveAction (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) override;

};


class Button : public Widget {
    enum ButtonState {
        BTN_NORMAL   = 0,
        BTN_PRESSED  = 1,
        BTN_FOCUSED  = 2,
        BTN_DISABLED = 3,
    };

    public:
    size_t width;
    size_t height;
    ButtonState state;

    explicit Button (double x, double y, size_t w, size_t h);

    virtual void MouseMoveAction (const Vec& mousepos) override;

    virtual bool MouseOnWidget (const Vec& mousepos) override;

};


class BtnMenu : public Widget {
    Button** buttons;
    size_t size;
    size_t capacity;

    public:

    BtnMenu();

    ~BtnMenu();

    void AddButton (Button* btn);

    virtual void RenderThis (sf::RenderWindow& sfwindow) const;
};


class ImgButton : public Button {
    Texture textures[4];

    public:

    explicit ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_ = nullptr);

    void SetTextures (const Texture* textures_);

    virtual void Render (RenderTarget& screen, const RegionSet& to_draw) const override;


    virtual void MousePressAction (const Vec& mousepos, MouseButtons mousebtn) override {};

    virtual void MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) override {};
};

class TxtButton : public Button {
    Text txt;

    public:

    explicit TxtButton (double x, double y, size_t w, size_t h, const Text& txt_);

    void SetText (const Text& txt);

    virtual void Render (RenderTarget& screen, const RegionSet& to_draw) const override;


    virtual void MousePressAction (const Vec& mousepos, MouseButtons mousebtn) override {};

    virtual void MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) override {};
};

//----------------------------------------------------------------------------------------------




class Tool {
    Vec start_pos;
    Vec last_pos;

    virtual void PaintOnPress ();
};


class ToolManager {

};

class Canvas : public Widget {
    bool drawing;

};

//-----------------------------------------------------------------------------------------------


void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num);

#endif
