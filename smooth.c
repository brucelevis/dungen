#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

static void counts(enum dg_cell_kind k, int *walls, int *floors, int *stones, int v)
{
    switch (k) {
        case dg_cell_floor:
            (*floors) += v;
            break;
        case dg_cell_wall:
            (*walls) += v;
            break;
        case dg_cell_stone:
            (*stones) += v;
            break;
    }
}

static void noise_smooth(dg_dungeon d, int x, int y, enum dg_cell_kind kind)
{
    /* walls, floors, stones */
    int w=0, f=0, s=0;

    /* count the current cell */
    counts(kind, &w, &f, &s, 1);

    /* add a random chance the cell will change
       chance should be stronger when cell is less certain
       edge cells should have a lower chance of change */
    int chance = (y > 0 ? 1 : 0)
               + (y < (d->h - 1) ? 1 : 0)
               - (x > 0 ? 0 : 1)
               - (x < (d->w - 1) ? 0 : 1);

    counts((enum dg_cell_kind)rnd_range(0, 3), &w, &f, &s, chance);

    /* top */
    if (y > 0) {
        if (x > 0) {
            counts(dg_get(d, x-1, y-1), &w, &f, &s, 1);
        }

        counts(dg_get(d, x, y-1), &w, &f, &s, 1);

        if (x < (d->w - 1)) {
            counts(dg_get(d, x+1, y-1), &w, &f, &s, 1);
        }
    }

    /* middle */
    if (x > 0) {
        counts(dg_get(d, x-1, y), &w, &f, &s, 1);
    }
    if (x < (d->w - 1)) {
        counts(dg_get(d, x+1, y), &w, &f, &s, 1);
    }

    /* bottom */
    if (y < d->h) {
        if (x > 0) {
            counts(dg_get(d, x-1, y+1), &w, &f, &s, 1);
        }

        counts(dg_get(d, x, y+1), &w, &f, &s, 1);

        if (x < (d->w - 1)) {
            counts(dg_get(d, x+1, y+1), &w, &f, &s, 1);
        }
    }

    /* set the cell to the highest count */
    dg_set(d, x, y,
        (w > f ? (w > s ? dg_cell_wall : dg_cell_stone) :
                          (f > s ? dg_cell_floor : dg_cell_stone)));
}

void dg_smooth(dg_dungeon d)
{
    dg_each(d, noise_smooth);
}
