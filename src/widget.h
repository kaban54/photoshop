#ifndef WIDGET_H
#define WIDGET_H

#include "vec.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <SFML/Graphics.hpp>

const size_t BASE_WIDGETMAN_CAP = 8;
const sf::Color WINDOW_BG_COLOR = sf::Color (128, 128, 128);

class Widget;

class WidgetManager {
    Widget** widgets;
    size_t size;
    size_t capacity;
    
    public:

    WidgetManager();
    
    ~WidgetManager();
    
    void AddWidget (Widget* widget);

    void Render (sf::RenderWindow& sfwindow) const;

};

class Widget {
    WidgetManager subwidgets;
    public:
    Vec pos;

    explicit Widget();

    explicit Widget (int x, int y);

    virtual void RenderThis (sf::RenderWindow& sfwindow) const = 0;    

    void Render (sf::RenderWindow& sfwindow) const;

    void AddSubWidget (Widget* wid);

    void Move (const Vec& vec);
};

class Window : public Widget {
    size_t width;
    size_t height;
    public:

    explicit Window (size_t w, size_t h);

    explicit Window (int x, int y, size_t w, size_t h);

    virtual void RenderThis (sf::RenderWindow& sfwindow) const override;
};


void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num);

#endif
