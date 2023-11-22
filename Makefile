CC = g++
CFLAGS += -DNDEBUG
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
OBJDIR = obj/
SRCDIR = src/

all: prog

prog: obj/main.o obj/vec2.o obj/widget.o obj/rendertarget.o obj/region.o obj/buttons.o obj/app.o\
	  obj/canvas.o obj/events.o obj/window.o obj/menu.o obj/tool.o obj/filter.o obj/editbox.o
	$(CC) -o prog obj/main.o obj/vec2.o obj/widget.o obj/rendertarget.o obj/region.o obj/buttons.o obj/app.o\
	  obj/canvas.o obj/events.o obj/window.o obj/menu.o obj/tool.o obj/filter.o obj/editbox.o $(SFMLFLAGS)

regtest: src/regtest.cpp src/vec.cpp src/widget.cpp src/rendertarget.cpp src/color.cpp src/filter.cpp\
		 src/region.cpp src/buttons.cpp src/drawing.cpp src/events.cpp src/menu.cpp
	$(CC) -o reg src/regtest.cpp src/vec.cpp src/widget.cpp src/rendertarget.cpp src/color.cpp src/filter.cpp\
				 src/region.cpp src/buttons.cpp src/drawing.cpp src/events.cpp src/window.cpp src/menu.cpp -DREGDEBUG $(SFMLFLAGS)

test: obj/test.o obj/mylist.o obj/region.o obj/vec.o src/mylist.h
	$(CC) -o test obj/test.o obj/mylist.o obj/region.o obj/vec.o $(SFMLFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean