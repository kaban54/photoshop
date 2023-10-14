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

const int W = 2160;
const int H = 1440;

void LoadTxtBtnTextures (Texture textures[4]);

void SetWidgets (Window& mainwin);

int main() {
    GLOBAL_FONT.loadFromFile (FONT_FILENAME);

    sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    //sfwindow.setFramerateLimit (300);

    Vec mousepos (0, 0);
    bool mouse_pressed = false;

    Window mainwin (100, 100, 1920, 1080);

    RenderTarget rt (W, H);
    SetWidgets (mainwin);
    mainwin.SetRenderTarget (&rt);

    //mainwin.Show();

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
                mainwin.MouseMove (mousepos);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                mainwin.MousePress (mousepos, MOUSE_LEFT);
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                mainwin.MouseRelease (mousepos, MOUSE_LEFT);
            }
        }
        
                
        sfwindow.clear(sf::Color (192, 192, 192));
        rt.ClearScreen (Color (192, 192, 192));
        mainwin.Render (rt, nullptr);
        rt.Display(sfwindow);
        sfwindow.display();
    }
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

    static sf::Text cols_txt;
    cols_txt.setFont(GLOBAL_FONT);
    cols_txt.setString("colors");
    cols_txt.setCharacterSize (30);
    static sf::Text tools_txt;
    tools_txt.setFont(GLOBAL_FONT);
    tools_txt.setString("tools");
    tools_txt.setCharacterSize (30);
    static sf::Text brush_txt;
    brush_txt.setFont(GLOBAL_FONT);
    brush_txt.setString("brush");
    brush_txt.setCharacterSize (30);
    static sf::Text rect_txt;
    rect_txt.setFont(GLOBAL_FONT);
    rect_txt.setString("rect");
    rect_txt.setCharacterSize (30);

    static Brush brush (25);
    static RectTool recttool;
    static ToolManager tm;
    tm.SetTool (&brush);
    tm.SetColor (Color (255, 0, 128));

    Window* win = new Window (200, 200, 610, 630);
    win -> AddSubWidget (new Canvas (5, 25, 600, 600, &tm));
    mainwin.AddSubWidget (win);
    win = new Window (1000, 100, 610, 630);
    win -> AddSubWidget (new Canvas (5, 25, 600, 600, &tm));
    mainwin.AddSubWidget (win);

    Texture textures[4];
    LoadTxtBtnTextures(textures);

    BtnChooseMenu* bm = new BtnChooseMenu (new TxtButton (5, 25, 200, 80, textures, Text(&tools_txt)));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, textures, Text(&brush_txt), &tm, &brush));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, textures, Text(& rect_txt), &tm, &recttool));
    mainwin.AddSubWidget (bm);

    bm = new BtnChooseMenu (new TxtButton (210, 25, 200, 80, textures, Text(&cols_txt)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, textures, &tm, Color(255, 0, 0)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, textures, &tm, Color(0, 255, 0)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, textures, &tm, Color(0, 0, 255)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, textures, &tm, Color(255, 0, 255)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, textures, &tm, Color(255, 255, 0)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, textures, &tm, Color(0, 255, 255)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, textures, &tm, Color(0, 0, 0)));
    bm -> AddButton (new ColorBtn (0, 0, 200, 80, textures, &tm, Color(255, 255, 255)));
    mainwin.AddSubWidget (bm);

}