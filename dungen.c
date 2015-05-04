#include <stdio.h>
#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

dg_dungeon dg_generate(int width, int height, dg_render_step fn)
{
    struct dgx_dungeon* d = malloc(sizeof(struct dgx_dungeon));
    d->w = width;
    d->h = height;
    d->generations = ((d->w + d->h) / 2) * 6;
    d->cells = malloc(sizeof(struct cell) * (d->w * d->h));

    int i = 0;
    for (int y=0; y < d->h; y++) {
        for (int x=0; x < d->w; x++) {
            struct cell *cell = &d->cells[i++];
            cell->x = x;
            cell->y = y;
            cell->kind = c_stone;
        }
    }

    // run the simulation
    seed_rng();

    // generate a worms
    struct worm* w = worm_create();
    w->x = width >> 1;
    w->y = height >> 1;

    // create a burrow the worm can call home
    //worm_burrow(d, w);
    worm_eat(d, w);
    worm_split(w);
    worm_split(w);

    // run the simulation for N steps
    int steps = 0;

    while (++steps < d->generations) {
        worm_tick(d, w);

        if (fn != NULL) {
            fn(d, steps);
        }
    }

    // each worm sleeps in a burrow
    //do {
    //    worm_burrow(d, w);
    //    w = w->segment;
    //} while (w != NULL);

    // final step
    fn(d, steps);

    // clean up
    return d;
}

void dg_each(dg_dungeon d, dg_each_cell fn)
{
    for (int y=0; y<d->h; y++) {
        for (int x=0; x<d->w; x++) {
          fn(x, y, CELL_AT(d, x, y).kind);
        }
    }
}

