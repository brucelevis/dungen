# DUNGEN

C dungeon generation library designed for easy static embedding.

## Installation / usage

`make libdungen.a` builds a static library you can link against.
The code should be easy to embed in another project.

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
    dg_dungeon d = dg_generate(WIDTH, HEIGHT, NULL);
    dg_each(d, print);

    return 0;
}
```

## SDL Demo

Make will build an animated SDL demo by default.

![screenshot](https://raw.github.com/jdeseno/dungen/master/screenshot.png)
