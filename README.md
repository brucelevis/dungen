# DUNGEN

C dungeon generation library designed for easy static embedding.

## Installation

`make libdungen.a` builds a static library you can link against.
The code should be easy to embed in another project.

## Usage

The API exposes an opaque dungeon structure that represents a grid and cells.
Different generation functions can be applied to a dungeon to create customizable effects.

## Example

``` c
#include <stdio.h>
#include "dungen.h"

#define WIDTH 60
#define HEIGHT 40

void print(int x, int y, enum dg_cell_kind k)
{
    switch(k) {
        case dg_cell_stone:
            printf(" ");
            break;
        case dg_cell_wall:
            printf("#");
            break;
        case dg_cell_floor:
            printf(".");
            break;
    }
    if (x == (WIDTH - 1)) {
        printf("\n");
    }
}

int main()
{
    dg_dungeon d = dg_create(WIDTH, HEIGHT, NULL);
    dg_worms(d);
    dg_each(d, print);
    dg_free(d);

    return 0;
}
```

## SDL Demo

Make will build an animated SDL demo by default.

![screenshot](https://raw.github.com/jdeseno/dungen/master/screenshot.png)
