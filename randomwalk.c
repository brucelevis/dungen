#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

void dg_randomwalk(dg_dungeon d)
{
    struct point p, dir;
    p.x = d->w / 2;
    p.y = d->h / 2;
    dir = dir_rnd();

    seed_rng();

    int steps = 0;
    while (++steps < d->generations)
    {
        dir_change(&dir);
        int x = p.x + dir.x;
        int y = p.y + dir.y;

        // try a new position until if we are inside the border
        while (x < 1 || y < 1 || x >= (d->w-1) || y >= (d->h-1)) {
            dir_change(&dir);
            x = p.x + dir.x;
            y = p.y + dir.y;
        }

        // move & clear
        p.x = x;
        p.y = y;
        CELL_AT(d, p.x, p.y).kind = dg_cell_floor;

        if (d->step_fn != NULL) {
            d->step_fn(d, steps);
        }
    }

    if (d->step_fn != NULL) {
        d->step_fn(d, steps);
    }
}
