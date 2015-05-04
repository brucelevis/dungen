CC=clang
CFLAGS+=-Wall -std=c99
CFLAGS+=-Wall -std=c99 $(shell pkg-config --cflags sdl2)
LDFLAGS+= $(shell pkg-config --libs sdl2)

OBJECTS=utils.o worm.o dungen.o

.DEFAULT: test
test: $(OBJECTS) test.o

debug: CFLAGS += -g
debug: clean test

release: CFLAGS += -O3 -DNDEBUG
release: clean test

.PHONY: clean
clean:
	rm -f test *.o
