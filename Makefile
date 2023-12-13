CC = g++
CFLAGS += -fPIC -Wall
LDFLAGS += -lstdc++
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
OBJDIR = obj/
SRCDIR = src/

all: app

app: obj/main.o obj/vec2.o obj/widget.o obj/rendertarget.o obj/region.o obj/buttons.o obj/app.o obj/controller.o\
	  obj/canvas.o obj/events.o obj/window.o obj/menu.o obj/tool.o obj/filter.o obj/editbox.o obj/texture.o obj/files.o
	$(CC) -o app obj/main.o obj/vec2.o obj/widget.o obj/rendertarget.o obj/region.o obj/buttons.o obj/app.o obj/controller.o\
	  obj/canvas.o obj/events.o obj/window.o obj/menu.o obj/tool.o obj/filter.o obj/editbox.o obj/texture.o obj/files.o $(SFMLFLAGS) $(LDFLAGS)

appv1: obj/stdv1/main.o obj/stdv1/vec2.o obj/stdv1/widget.o obj/stdv1/rendertarget.o obj/stdv1/region.o obj/stdv1/buttons.o obj/stdv1/app.o obj/stdv1/controller.o\
	  obj/stdv1/canvas.o obj/stdv1/events.o obj/stdv1/window.o obj/stdv1/menu.o obj/stdv1/tool.o obj/stdv1/filter.o obj/stdv1/editbox.o obj/stdv1/texture.o obj/stdv1/files.o
	$(CC) -o appv1 obj/stdv1/main.o obj/stdv1/vec2.o obj/stdv1/widget.o obj/stdv1/rendertarget.o obj/stdv1/region.o obj/stdv1/buttons.o obj/stdv1/app.o obj/stdv1/controller.o\
	  obj/stdv1/canvas.o obj/stdv1/events.o obj/stdv1/window.o obj/stdv1/menu.o obj/stdv1/tool.o obj/stdv1/filter.o obj/stdv1/editbox.o obj/stdv1/texture.o obj/stdv1/files.o $(SFMLFLAGS) $(LDFLAGS)

bwplug: obj/texture.o obj/vec2.o src/test_plugin.cpp
	g++ -c -o obj/bwplug.o src/bwplug.cpp $(CFLAGS)
	gcc -shared -o plugins/bwplug.so obj/texture.o obj/vec2.o obj/bwplug.o $(LDFLAGS)

rect_tool: obj/texture.o obj/vec2.o src/rect_tool.cpp
	g++ -c -o obj/rect_tool.o src/rect_tool.cpp $(CFLAGS)
	gcc -shared -o plugins/rect_tool.so obj/texture.o obj/vec2.o obj/rect_tool.o $(LDFLAGS)

spline_tool: obj/texture.o obj/vec2.o obj/catmullrom.o src/spline_tool.cpp
	g++ -c -o obj/spline_tool.o src/spline_tool.cpp $(CFLAGS)
	gcc -shared -o plugins/spline_tool.so obj/texture.o obj/vec2.o obj/catmullrom.o obj/spline_tool.o $(LDFLAGS)


$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm obj/*.o -f
	rm obj/stdv1/*.o -f
	clear
	
.PHONY: clean