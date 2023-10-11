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
    virtual void Render (RenderTarget& screen) const = 0;
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

    void Render (RenderTarget& screen) const;

    void Move (const Vec& vec);

    void MousePress (const Vec& mousepos, MouseButtons mousebtn);

    void MouseRelease (const Vec& mousepos, MouseButtons mousebtn);

    void MouseMove (const Vec& mousepos);
};

class Widget : public Renderable {

    Widget* parent;
    WidgetManager subwidgets;

    public:
    Vec pos;
    bool visible;

    explicit Widget();

    explicit Widget (int x, int y, size_t subw_cap = BASE_WIDGETMAN_CAP);
    
    void AddSubWidget (Widget* wid);

    void RenderSubWidgets (RenderTarget& screen) const;

    void Move (const Vec& vec);


    void MousePress (const Vec& mousepos, MouseButtons mousebtn);

    void MouseRelease (const Vec& mousepos, MouseButtons mousebtn);

    void MouseMove (const Vec& mousepos);

    virtual void MousePressAction (const Vec& mousepos, MouseButtons mousebtn) = 0;

    virtual void MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) = 0;

    virtual void MouseMoveAction (const Vec& mousepos) = 0;

    virtual bool MouseOnWidget (const Vec& mousepos) = 0;

};


class Window : public Widget {
    size_t width;
    size_t height;
    RegionSet regset;

    bool is_moving;
    Vec hold_pos;

    public:

    explicit Window (size_t w, size_t h);

    explicit Window (int x, int y, size_t w, size_t h);

    virtual void Render (RenderTarget& screen) const override;


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

    size_t width;
    size_t height;
    ButtonState state;

    explicit Button (double x, double y, size_t w, size_t h);



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


class TextButton : public Button {

};


void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num);

#endif
