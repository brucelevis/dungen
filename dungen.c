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
    d->rooms = NULL;
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
            fn(d, x, y, CELL_AT(d, x, y).kind);
        }
    }
}

void dg_each_room(dg_dungeon d, dg_each_rect fn)
{
    struct rect_l *r = d->rooms;
    while (r != NULL) {
        fn(d, r->rect.x, r->rect.y, r->rect.w, r->rect.h);
    }
}

void dg_print(dg_dungeon d, int x, int y, enum dg_cell_kind kind)
{
    switch (kind) {
        case dg_cell_stone:
            putchar(' ');
            break;
        case dg_cell_wall:
            putchar('#');
            break;
        case dg_cell_floor:
            putchar('.');
            break;
    }
    if (x == (d->w - 1)) {
        putchar('\n');
    }
}
