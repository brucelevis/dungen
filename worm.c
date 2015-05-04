#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

struct worm *worm_create()
{
    struct worm *w = malloc(sizeof(struct worm));
    w->x = 1;
    w->y = 1;
    w->ticks = 0;
    w->dead = 0;
    w->segment = NULL;
    w->dir = dir_rnd();

    return w;
}

void worm_eat(dg_dungeon d, struct worm *w)
{
    CELL_AT(d, w->x, w->y).kind = c_floor;
}

void worm_split(struct worm *w)
{
    if (w->segment == NULL) {
        w->segment = worm_create();
        w->segment->x = w->x;
        w->segment->y = w->y;
    } else {
        worm_split(w->segment);
    }
}

void worm_burrow(dg_dungeon d, struct worm *wrm)
{
    // pick a random size
    int w = rnd_range(4, 12);
    int h = rnd_range(4, 12);

    // center the rect on the worm
    int start_x = wrm->x - (w / 2);
    int start_y = wrm->y - (h / 2);

    // stay away from the bordering stone
    if (start_x < 1) { start_x = 1; }
    if (start_y < 1) { start_y = 1; }
    if (start_x + w > d->w) { start_x = d->w - (w + 1); }
    if (start_y + h > d->h) { start_y = d->h - (h + 1); }

    // clear every cell inside the rect
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            CELL_AT(d, (start_x+x), (start_y+y)).kind = c_floor;
        }
    }
}

void worm_tick(dg_dungeon d, struct worm *w)
{
    int generations = d->generations;

    int life_ticks = generations / 3;

    w->ticks++;
    if (w->ticks++ >= life_ticks) {
        w->dead = 1;
    }

    if (!w->dead) {

        int x = w->x + w->dir.x;
        int y = w->y + w->dir.y;

        // try a new position until if we are inside the border
        while (x < 1 || y < 1 || x >= (d->w-1) || y >= (d->h-1)) {
            dir_change(&w->dir);
            x = w->x + w->dir.x;
            y = w->y + w->dir.y;
        }

        int virility = life_ticks / (w->ticks * 1.3);
        int keel = life_ticks / (w->ticks / 1.1);

        // split a new worm segment
        if (w->segment == NULL && rnd_coinflip(virility)) {
            // worm_burrow(d, w);
            worm_split(w);
        }

        // move and eat the cell
        w->x = x;
        w->y = y;
        worm_eat(d, w);

        if (rnd_coinflip(keel)) {
            dir_change(&w->dir);
        }
    }

    // tick child segments
    if (w->segment != NULL) {
        worm_tick(d, w->segment);
    }
}
