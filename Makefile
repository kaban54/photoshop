CC = g++
CFLAGS += -DNDEBUG
SFMLFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
OBJDIR = obj/
SRCDIR = src/

all: prog

prog: obj/main.o obj/vec.o obj/widget.o
	$(CC) -o prog obj/main.o obj/vec.o obj/widget.o $(SFMLFLAGS)

test: obj/test.o obj/mylist.o obj/region.o obj/vec.o src/mylist.h
	$(CC) -o test obj/test.o obj/mylist.o obj/region.o obj/vec.o $(SFMLFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm obj/*.o -f
	clear
	
.PHONY: clean