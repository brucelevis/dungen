CFLAGS+=-Wall -std=c99

OBJECTS=chunky.o \
	dungen.o \
	fill.o \
	maze.o \
	noise.o \
	randomwalk.o \
	shrinker.o \
        utils.o \
	voronoi.o \
	worm.o

.DEFAULT:
libdungen.a: $(OBJECTS)
	ar -cr libdungen.a $(OBJECTS)

debug: CFLAGS += -g
debug: clean libdungen.a

release: CFLAGS += -O3 -DNDEBUG
release: clean libdungen.a

demo: CFLAGS+=$(shell pkg-config --cflags sdl2)
demo: LDFLAGS+=$(shell pkg-config --libs sdl2)
demo: libdungen.a demo.o

.PHONY: clean
clean:
	rm -f demo libdungen.a *.o
