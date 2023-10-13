#include "widget.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "rendertarget.h"
#include <unistd.h>

const char* FONT_FILENAME = "fonts/font.ttf";
sf::Font GLOBAL_FONT;

const int W = 2160;
const int H = 1440;

void LoadTestTextures (Texture test_textures[4]);

int main() {
    GLOBAL_FONT.loadFromFile (FONT_FILENAME);

    sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    sfwindow.setFramerateLimit (300);

    Vec mousepos (0, 0);
    bool mouse_pressed = false;

    Window mainwin (200, 300, 800, 800);


    RenderTarget rt (W, H);
    mainwin.SetRenderTarget (&rt);

    mainwin.AddSubWidget (new Window (100, 100, 200, 300));

/*
    Texture test_textures[4];
    sf::Texture sf_test_textures[4];
    sf_test_textures[0].loadFromFile ("textures/btn40.png");
    sf_test_textures[1].loadFromFile ("textures/btn41.png");
    sf_test_textures[2].loadFromFile ("textures/btn42.png");

    test_textures[0].sftexture = sf_test_textures + 0;
    test_textures[1].sftexture = sf_test_textures + 1;
    test_textures[2].sftexture = sf_test_textures + 2;
    test_textures[3].sftexture = nullptr;

    mainwin.AddSubWidget (new ImgButton (50, 50, 80, 80, test_textures));
*/
    std::cerr << "OK1!\n";
    mainwin.Show();

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
        rt.Display(sfwindow);
        sfwindow.display();

        //std::cerr << mainwin.regset.regions.GetSize() << "\n";
        //sleep (1);
    }
    return 0;
}