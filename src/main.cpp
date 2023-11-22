#include <cmath>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "rendertarget.h"
#include <unistd.h>
#include "menu.h"
#include "window.h"
#include "canvas.h"

const char* FONT_FILENAME = "fonts/font2.ttf";
const char* EVENTLOG_FILENAME = "logs/eventlog";

const int W = 2160;
const int H = 1440;

void LoadTxtBtnTextures (Texture textures[4]);

void SetWidgets (Window& mainwin, EventManager& ev_man);

int main() {
    sf::Font fnt;
    fnt.loadFromFile (FONT_FILENAME);
    FILE* eventlogfile = fopen (EVENTLOG_FILENAME, "w");
    EventLogger eventlogger (eventlogfile);
    EventManager event_man;
    event_man.registerObject(&eventlogger);

    // sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    sf::RenderWindow sfwindow (sf::VideoMode (2000, 1200), "PHOTOSHOP228");
    sfwindow.setFramerateLimit (120);

    RenderTarget rt (W, H);
    rt.SetFont(&fnt);
    Background* bg = new Background (W, H);
    bg -> SetRenderTarget (&rt);
    Window* mainwin = new Window (100, 100, 1920, 1080);
    
    SetWidgets (*mainwin, event_man);
    bg -> registerSubWidget(mainwin);
    
    event_man.registerObject(bg);
    Vec2 mousepos (0, 0);

    sf::Clock clk;
    uint64_t last_time = 0;

    while (sfwindow.isOpen()) {
        sf::Event event;

        while (sfwindow.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                sfwindow.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    sfwindow.close();
                KeyboardContext kstate = {event.key.alt, event.key.control, event.key.shift, (Key)event.key.code};
                event_man.onKeyboardPress (kstate);   
            }

            if (event.type == sf::Event::KeyReleased) {
                KeyboardContext kstate = {event.key.alt, event.key.control, event.key.shift, (Key)event.key.code};
                event_man.onKeyboardRelease (kstate);
            }

            if (event.type == sf::Event::MouseMoved) {
                mousepos.x = event.mouseMove.x;
                mousepos.y = event.mouseMove.y;
                event_man.onMouseMove (MouseContext(mousepos, MouseButton::Left));
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left )
                    event_man.onMousePress (MouseContext(mousepos, MouseButton::Left));
                if (event.mouseButton.button == sf::Mouse::Right)
                    event_man.onMousePress (MouseContext(mousepos, MouseButton::Right));
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left )
                    event_man.onMouseRelease (MouseContext(mousepos, MouseButton::Left));
                if (event.mouseButton.button == sf::Mouse::Right)
                    event_man.onMouseRelease (MouseContext(mousepos, MouseButton::Right));
            }
        }

        uint64_t current_time = clk.getElapsedTime().asMicroseconds();
        if (current_time - last_time >= 10000) {
            event_man.onClock (current_time - last_time);
            last_time = current_time;
        }

        sfwindow.clear(sf::Color (192, 192, 192));
        rt.SfDisplay(sfwindow);
        sfwindow.display();
    }

    delete bg;
    fclose (eventlogfile);
    return 0;
}

// void LoadTxtBtnTextures (Texture textures[4]) {
//     static sf::Texture sftextures[4];
//     sftextures[0].loadFromFile ("textures/e0.png");
//     sftextures[1].loadFromFile ("textures/e1.png");
//     sftextures[2].loadFromFile ("textures/e2.png");
//     textures[0].sftexture = sftextures + 0;
//     textures[1].sftexture = sftextures + 1;
//     textures[2].sftexture = sftextures + 2;
//     textures[3].sftexture = nullptr;
// }

void SetWidgets (Window& mainwin, EventManager& ev_man) {
    static Brush brush (25);
    static ToolManager tm;
    tm.setTool (&brush);
    tm.setColor (Color (255, 0, 128));

    VerticalMenu* tools_vm = new VerticalMenu (405, 105);
    tools_vm -> AddButton (new ToolBtn (0, 0, 200, 80, "brush", 30, &tm, &brush));

    VerticalMenu* cols_vm = new VerticalMenu (405, 185);
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 0, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 255, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 0, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 0, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 255, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 255, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 0, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 255, 255)));

    static FilterManager fm;
    static TestFilter test_filter;

    VerticalMenu* filters_vm = new VerticalMenu (405, 265);
    filters_vm -> AddButton (new FilterBtn (0, 0, 200, 80, "test filter", 30, &fm, &test_filter, &ev_man, &mainwin));

    VerticalMenu* vm = new VerticalMenu (205, 105);
    vm -> registerSubWidget (tools_vm);
    vm -> registerSubWidget (cols_vm);
    vm -> registerSubWidget (filters_vm);
    vm -> AddButton (new MenuBtn (0, 0, 200, 80,  "tools" , 30,   tools_vm));
    vm -> AddButton (new MenuBtn (0, 0, 200, 80, "colors" , 30,    cols_vm));
    vm -> AddButton (new MenuBtn (0, 0, 200, 80, "filters", 30, filters_vm));
    mainwin.registerSubWidget (vm);
    mainwin.registerSubWidget (new MenuBtn (205, 25, 200, 80, "edit", 30, vm));

    Window* win = new Window (200, 200, 610, 630);
    win -> registerSubWidget (new Canvas (5, 25, 600, 600, &tm, &fm));
    mainwin.registerSubWidget (win);
    win = new Window (1000, 100, 610, 630);
    win -> registerSubWidget (new Canvas (5, 25, 600, 600, &tm, &fm));
    mainwin.registerSubWidget (win);
}