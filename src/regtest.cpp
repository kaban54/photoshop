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

    Vec mousepos (0, 0);
    bool mouse_pressed = false;

    Window mainwin (500, 500, 900, 900);

    RenderTarget rt (W, H);
    SetWidgets (mainwin);
    mainwin.SetRenderTarget (&rt);

    mainwin.Show();
    mainwin.Render(rt, &(mainwin.regset));
    mainwin.RenderSubWidgets(rt);

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
                if (event.mouseButton.button == sf::Mouse::Left ) mainwin.MousePress (mousepos, MOUSE_LEFT );
                if (event.mouseButton.button == sf::Mouse::Right) mainwin.MousePress (mousepos, MOUSE_RIGHT);
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left ) mainwin.MouseRelease (mousepos, MOUSE_LEFT );
                if (event.mouseButton.button == sf::Mouse::Right) mainwin.MouseRelease (mousepos, MOUSE_RIGHT);
            }
        }
        sfwindow.clear(sf::Color (192, 192, 192));
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
    mainwin.AddSubWidget (new Window (100, 140, 300, 300));
    // mainwin.AddSubWidget (new Window (500, 100, 300, 300));
    
    // Window* win = new Window (100, 500, 400, 200);
    // win->AddSubWidget (new Window (100, 50, 200, 100));
    // mainwin.AddSubWidget(win);


    static sf::Text    tools_txt ("tools"   , GLOBAL_FONT, 30);
    static sf::Text    brush_txt ("brush"   , GLOBAL_FONT, 30);
    static sf::Text     rect_txt ("rect"    , GLOBAL_FONT, 30);

    static Brush brush (25);
    static RectTool recttool;
    static ToolManager tm;
    tm.SetTool (&brush);
    tm.SetColor (Color (255, 0, 128));

    Texture textures[4];
    LoadTxtBtnTextures(textures);

    BtnChooseMenu* bm = new BtnChooseMenu (5, 25, 200, 80, textures, Text(&tools_txt));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, textures, Text(&   brush_txt), &tm, &brush));
    bm -> AddButton (new ToolBtn (0, 0, 200, 80, textures, Text(&    rect_txt), &tm, &recttool));
    mainwin.AddSubWidget (bm);
}