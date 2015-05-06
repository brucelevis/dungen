# DUNGEN

C dungeon generation library designed for easy embedding.

``` c
#include <stdio.h>
#include "dungen.h"

void print(int x, int y, enum dg_cell_kind k)
{
    switch(k) {
        case c_stone:
            printf(" ");
            break;
        case c_wall:
            printf("#");
            break;
        case c_floor:
            printf(".");
            break;
    }
}

int main()
{
    dg_dungeon d = dg_generate(60, 40, NULL);

    dg_each(d, print);

    return 0;
}
```

``` sh
$ cc dungen.o demo.c -o demo
$ ./demo
```
