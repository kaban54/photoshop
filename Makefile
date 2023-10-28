CC = g++
CFLAGS += -DNDEBUG
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
OBJDIR = obj/
SRCDIR = src/

all: prog

prog: obj/main.o obj/vec.o obj/widget.o obj/rendertarget.o obj/color.o \
	  obj/region.o obj/buttons.o obj/drawing.o obj/events.o
	$(CC) -o prog obj/main.o obj/vec.o obj/widget.o obj/rendertarget.o obj/color.o \
			      obj/region.o obj/buttons.o obj/drawing.o obj/events.o $(SFMLFLAGS)

regtest: src/regtest.cpp src/vec.cpp src/widget.cpp src/rendertarget.cpp src/color.cpp \
		 src/region.cpp src/buttons.cpp src/drawing.cpp src/events.cpp
	$(CC) -o reg src/regtest.cpp src/vec.cpp src/widget.cpp src/rendertarget.cpp src/color.cpp \
				 src/region.cpp src/buttons.cpp src/drawing.cpp src/events.cpp -DREGDEBUG $(SFMLFLAGS)

test: obj/test.o obj/mylist.o obj/region.o obj/vec.o src/mylist.h
	$(CC) -o test obj/test.o obj/mylist.o obj/region.o obj/vec.o $(SFMLFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean