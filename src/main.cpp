#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "rendertarget.h"
#include <unistd.h>
#include "menu.h"
#include "drawing.h"
#include "window.h"
#include "filter.h"
#include "editbox.h"

const char* FONT_FILENAME = "fonts/font2.ttf";
Font GLOBAL_FONT;

const char* EVENTLOG_FILENAME = "logs/eventlog";

const int W = 2160;
const int H = 1440;

void LoadTxtBtnTextures (Texture textures[4]);

void SetWidgets (Window& mainwin);

int main() {
    sf::Font fnt;
    fnt.loadFromFile (FONT_FILENAME);
    GLOBAL_FONT.sffont = &fnt;
    FILE* eventlogfile = fopen (EVENTLOG_FILENAME, "w");
    EventLogger eventlogger (eventlogfile);

    // sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    sf::RenderWindow sfwindow (sf::VideoMode (2000, 1200), "PHOTOSHOP228");
    sfwindow.setFramerateLimit (120);

    Vec mousepos (0, 0);

    RenderTarget rt (W, H);
    Background bg (W, H);
    bg.SetRenderTarget (&rt);
    Window* mainwin = new Window (100, 100, 1920, 1080);
    SetWidgets (*mainwin);
    bg.AddSubWidget(mainwin);

    bg.Show();

    EventManager event_man;
    event_man.AddObject(&bg);
    event_man.AddObject(&eventlogger);

    TestFilter tf;


    while (sfwindow.isOpen()) {
        sf::Event event;

        while (sfwindow.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                sfwindow.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    sfwindow.close();
                KeyboardState kstate ((KeyboardKey)event.key.code, event.key.alt, event.key.control, event.key.shift);
                event_man.KeyboardPress (kstate);   
            }

            if (event.type == sf::Event::KeyReleased) {
                KeyboardState kstate ((KeyboardKey)event.key.code, event.key.alt, event.key.control, event.key.shift);
                event_man.KeyboardRelease (kstate);
            }

            if (event.type == sf::Event::MouseMoved) {
                mousepos.x = event.mouseMove.x;
                mousepos.y = event.mouseMove.y;
                event_man.MouseMove (MouseState(mousepos));
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left )
                    event_man.MousePress (MouseState(mousepos, MOUSE_LEFT ));
                if (event.mouseButton.button == sf::Mouse::Right)
                    event_man.MousePress (MouseState(mousepos, MOUSE_RIGHT));
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left )
                    event_man.MouseRelease (MouseState(mousepos, MOUSE_LEFT ));
                if (event.mouseButton.button == sf::Mouse::Right)
                    event_man.MouseRelease (MouseState(mousepos, MOUSE_RIGHT));
            }
        }
        
        sfwindow.clear(sf::Color (192, 192, 192));
        rt.Display(sfwindow);
        sfwindow.display();
    }

    fclose (eventlogfile);
    return 0;
}

void LoadTxtBtnTextures (Texture textures[4]) {
    static sf::Texture sftextures[4];
    sftextures[0].loadFromFile ("textures/e0.png");
    sftextures[1].loadFromFile ("textures/e1.png");
    sftextures[2].loadFromFile ("textures/e2.png");
    textures[0].sftexture = sftextures + 0;
    textures[1].sftexture = sftextures + 1;
    textures[2].sftexture = sftextures + 2;
    textures[3].sftexture = nullptr;
}

void SetWidgets (Window& mainwin) {

    static Text     edit_txt ("edit"    , GLOBAL_FONT, 30);
    static Text     cols_txt ("colors"  , GLOBAL_FONT, 30);
    static Text    tools_txt ("tools"   , GLOBAL_FONT, 30);
    static Text    brush_txt ("brush"   , GLOBAL_FONT, 30);
    static Text     rect_txt ("rect"    , GLOBAL_FONT, 30);
    static Text     line_txt ("line"    , GLOBAL_FONT, 30);
    static Text  ellipse_txt ("ellipse" , GLOBAL_FONT, 30);
    static Text polyline_txt ("polyline", GLOBAL_FONT, 30);

    static Brush brush (25);
    static RectTool recttool;
    static LineTool linetool;
    static EllipseTool elltool;
    static PolyLine polyline;
    static ToolManager tm;
    tm.SetTool (&brush);
    tm.SetColor (Color (255, 0, 128));

    static Text filter_test_txt ("filter test", GLOBAL_FONT, 30);
    static TestFilter test_filter;
    static FilterManager fm;
    fm.SetFilter (&test_filter);

    mainwin.AddSubWidget (new FilterBtn (600, 100, 300, 100, filter_test_txt, &fm, &test_filter));


    VerticalMenu* tools_vm = new VerticalMenu (405, 105);
    tools_vm -> AddButton (new ToolBtn (0, 0, 200, 80,    brush_txt, &tm, &brush));
    tools_vm -> AddButton (new ToolBtn (0, 0, 200, 80,     rect_txt, &tm, &recttool));
    tools_vm -> AddButton (new ToolBtn (0, 0, 200, 80,     line_txt, &tm, &linetool));
    tools_vm -> AddButton (new ToolBtn (0, 0, 200, 80,  ellipse_txt, &tm, &elltool));
    tools_vm -> AddButton (new ToolBtn (0, 0, 200, 80, polyline_txt, &tm, &polyline));
    // mainwin.AddSubWidget (tools_vm);
    //mainwin.AddSubWidget (new MenuBtn (5, 25, 200, 80, Text(&tools_txt), vm));

    VerticalMenu* cols_vm = new VerticalMenu (405, 185);
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 0, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 255, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 0, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 0, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 255, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 255, 255)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 0, 0)));
    cols_vm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 255, 255)));
    // mainwin.AddSubWidget (cols_vm);
    //mainwin.AddSubWidget (new MenuBtn (205, 25, 200, 80, Text(&cols_txt), vm));

    VerticalMenu* vm = new VerticalMenu (205, 105);
    vm -> AddSubWidget (tools_vm);
    vm -> AddSubWidget (cols_vm);
    vm -> AddButton (new MenuBtn (0, 0, 200, 80, tools_txt, tools_vm));
    vm -> AddButton (new MenuBtn (0, 0, 200, 80,  cols_txt,  cols_vm));
    mainwin.AddSubWidget (vm);
    mainwin.AddSubWidget (new MenuBtn (205, 25, 200, 80, edit_txt, vm));

    Window* win = new Window (200, 200, 610, 630);
    win -> AddSubWidget (new Canvas (5, 25, 600, 600, &tm, &fm));
    mainwin.AddSubWidget (win);
    win = new Window (1000, 100, 610, 630);
    win -> AddSubWidget (new Canvas (5, 25, 600, 600, &tm, &fm));
    mainwin.AddSubWidget (win);

    mainwin.AddSubWidget (new EditBox (100, 900, 600, 80, GLOBAL_FONT, 30));
}