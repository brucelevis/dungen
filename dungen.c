#include <stdio.h>
#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

dg_dungeon dg_create(int width, int height, dg_render_step step_fn)
{
    struct dgx_dungeon* d = malloc(sizeof(struct dgx_dungeon));

    d->w = width;
    d->h = height;
    d->generations = ((d->w + d->h) / 2) * 10;
    d->cells = malloc(sizeof(struct cell) * d->w * d->h);
    d->step_fn = step_fn;

    int i = 0;
    for (int y=0; y < d->h; y++) {
        for (int x=0; x < d->w; x++) {
            struct cell *cell = &d->cells[i++];
            cell->x = x;
            cell->y = y;
            cell->kind = dg_cell_stone;
        }
    }

    return d;
}

void dg_free(dg_dungeon d)
{
    free(d);
}

void dg_each(dg_dungeon d, dg_each_cell fn)
{
    for (int y=0; y<d->h; y++) {
        for (int x=0; x<d->w; x++) {
            fn(x, y, CELL_AT(d, x, y).kind);
        }
    }
}
