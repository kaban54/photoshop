CC = g++
CFLAGS += -DNDEBUG -fPIC -Wall
LDFLAGS += -lstdc++
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
OBJDIR = obj/
SRCDIR = src/

all: app

app: obj/main.o obj/vec2.o obj/widget.o obj/rendertarget.o obj/region.o obj/buttons.o obj/app.o\
	  obj/canvas.o obj/events.o obj/window.o obj/menu.o obj/tool.o obj/filter.o obj/editbox.o obj/texture.o
	$(CC) -o app obj/main.o obj/vec2.o obj/widget.o obj/rendertarget.o obj/region.o obj/buttons.o obj/app.o\
	  obj/canvas.o obj/events.o obj/window.o obj/menu.o obj/tool.o obj/filter.o obj/editbox.o obj/texture.o $(SFMLFLAGS) $(LDFLAGS)


plug1: obj/texture.o obj/vec2.o src/test_plugin.cpp
	g++ -c -o obj/plug1.o src/test_plugin.cpp $(CFLAGS)
	gcc -shared -o plugins/plug1.so obj/texture.o obj/vec2.o obj/plug1.o $(LDFLAGS) $(SFMLFLAGS)


$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean