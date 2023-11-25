#include <cmath>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <dlfcn.h>
#include "app.h"

const char* FONT_FILENAME = "fonts/font2.ttf";
const char* EVENTLOG_FILENAME = "logs/eventlog";

const int W = 2000;
const int H = 1200;

typedef plugin::Plugin* (*getInstance_t)(plugin::App *app);

int main() {
    sf::Font fnt;
    fnt.loadFromFile (FONT_FILENAME);
    RenderTarget rt (W, H);
    rt.SetFont(&fnt);

    FILE* eventlogfile = fopen (EVENTLOG_FILENAME, "w");
    EventLogger eventlogger (eventlogfile);
    EventManager event_man;
    event_man.registerObject (&eventlogger);

    // sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228", sf::Style::Fullscreen);
    sf::RenderWindow sfwindow (sf::VideoMode (W, H), "PHOTOSHOP228");
    sfwindow.setFramerateLimit (120);

    MyApp app (W, H, &event_man, &rt);
    app.SetupWidgets();


    void *lib = dlopen("plugins/Lol.so", RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);
	fprintf(stderr, "dll = %p\n", lib);

    if (!lib) {
        fputs (dlerror(), stderr);
        return 1;
    }

    getInstance_t get_inst = (getInstance_t) dlsym (lib, "getInstance");
    fprintf(stderr, "get = %p\n", get_inst);
    app.AddPlugin(get_inst(&app));

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

    fclose (eventlogfile);
    return 0;
}
