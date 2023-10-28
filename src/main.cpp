#include "widget.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "rendertarget.h"
#include <unistd.h>
#include "buttons.h"
#include "drawing.h"

const char* FONT_FILENAME = "fonts/font.ttf";
sf::Font GLOBAL_FONT;

const char* EVENTLOG_FILENAME = "logs/eventlog";

const int W = 2160;
const int H = 1440;

void LoadTxtBtnTextures (Texture textures[4]);

void SetWidgets (Window& mainwin);

int main() {
    GLOBAL_FONT.loadFromFile (FONT_FILENAME);
    FILE* eventlogfile = fopen (EVENTLOG_FILENAME, "w");
    EventLogger eventlogger (eventlogfile);

    sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    sfwindow.setFramerateLimit (120);

    Vec mousepos (0, 0);

    Background bg (W, H);
    Window* mainwin = new Window (100, 100, 1920, 1080);

    RenderTarget rt (W, H);
    SetWidgets (*mainwin);
    bg.AddSubWidget(mainwin);
    bg.SetRenderTarget (&rt);

    bg.Show();
    bg.Render(rt, bg.GetRegset());
    bg.RenderSubWidgets(rt);

    EventManager event_man;
    event_man.AddObject(&bg);
    event_man.AddObject(&eventlogger);

    while (sfwindow.isOpen()) {
        sf::Event event;

        while (sfwindow.pollEvent (event)) {
            if (event.type == sf::Event::Closed)
                sfwindow.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    sfwindow.close();
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

    static sf::Text     cols_txt ("colors"  , GLOBAL_FONT, 30);
    static sf::Text    tools_txt ("tools"   , GLOBAL_FONT, 30);
    static sf::Text    brush_txt ("brush"   , GLOBAL_FONT, 30);
    static sf::Text     rect_txt ("rect"    , GLOBAL_FONT, 30);
    static sf::Text     line_txt ("line"    , GLOBAL_FONT, 30);
    static sf::Text  ellipse_txt ("ellipse" , GLOBAL_FONT, 30);
    static sf::Text polyline_txt ("polyline", GLOBAL_FONT, 30);

    static Brush brush (25);
    static RectTool recttool;
    static LineTool linetool;
    static EllipseTool elltool;
    static PolyLine polyline;
    static ToolManager tm;
    tm.SetTool (&brush);
    tm.SetColor (Color (255, 0, 128));

    BtnChooseMenu* bm = new BtnChooseMenu (5, 25, 200, 80, Text(&tools_txt));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, Text(&   brush_txt), &tm, &brush));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, Text(&    rect_txt), &tm, &recttool));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, Text(&    line_txt), &tm, &linetool));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, Text(& ellipse_txt), &tm, &elltool));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, Text(&polyline_txt), &tm, &polyline));
    mainwin.AddSubWidget (bm);

    bm = new BtnChooseMenu (205, 25, 200, 80, Text(&cols_txt));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 0, 0)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 255, 0)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 0, 255)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 0, 255)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 255, 0)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 255, 255)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(0, 0, 0)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, &tm, Color(255, 255, 255)));
    mainwin.AddSubWidget (bm);

    Window* win = new Window (200, 200, 610, 630);
    win -> AddSubWidget (new Canvas (5, 25, 600, 600, &tm));
    mainwin.AddSubWidget (win);
    win = new Window (1000, 100, 610, 630);
    win -> AddSubWidget (new Canvas (5, 25, 600, 600, &tm));
    mainwin.AddSubWidget (win);
}