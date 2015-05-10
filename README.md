# DUNGEN

C dungeon generation library designed for easy static embedding.

``` c
#include <stdio.h>
#include "dungen.h"

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
}

int main()
{
    dg_dungeon d = dg_generate(60, 40, NULL);

    dg_each(d, print);

    return 0;
}
```

