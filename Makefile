CFLAGS+=-Wall -std=c99

.DEFAULT: demo
demo: CFLAGS+=$(shell pkg-config --cflags sdl2)
demo: LDFLAGS+=$(shell pkg-config --libs sdl2)
demo: libdungen.a demo.o

OBJECTS=utils.o worm.o dungen.o smoother.o

libdungen.a: $(OBJECTS)
	ar -cr libdungen.a $(OBJECTS)

debug: CFLAGS += -g
debug: clean demo

release: CFLAGS += -O3 -DNDEBUG
release: clean demo

.PHONY: clean
clean:
	rm -f demo libdungen.a *.o
