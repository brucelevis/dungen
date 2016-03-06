#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

struct worm {
    int x;
    int y;
    struct point dir;
    int ticks;
    int dead;
    int segment_depth;
    struct worm* segment;
};

static void worm_init(struct worm *worm)
{
    worm->x = 0;
    worm->y = 0;
    worm->ticks = 0;
    worm->dead = 0;
    worm->dir = dir_rnd();
    worm->segment_depth = 0;
    worm->segment = NULL;
}

static struct worm *worm_create()
{
    struct worm *worm;

    worm = DG_ALLOC(sizeof(struct worm));
    if (worm == NULL) {
        dg_panic("(internal) worm_create: out of memory");
    }
    worm_init(worm);

    return worm;
}

static void worm_free(struct worm *worm)
{
    if (worm->segment != NULL)
    {
        worm_free(worm->segment);
    }

    DG_FREE(worm);
    worm = NULL;
}

static void worm_eat(dg_dungeon d, const struct worm *w)
{
    dg_set(d, w->x, w->y, dg_cell_floor);
}

static void worm_split(struct worm *w)
{
    if (w->segment == NULL) {
        w->segment = worm_create();
        w->segment->x = w->x;
        w->segment->y = w->y;
        w->segment->segment_depth = w->segment_depth + 1;
    } else {
        worm_split(w->segment);
    }
}

static void worm_tick(dg_dungeon d, struct worm *w)
{
    int depth = w->segment_depth + 1;
    int life_ticks = 10 + (depth / 2 == 0 ? 6 : 0);
    int virility   = (depth / 4);
    int keel       = 6;

    w->ticks++;
    if (w->ticks++ >= life_ticks) {
        w->dead = 1;
    }

    if (!w->dead) {

        int x = w->x + w->dir.x;
        int y = w->y + w->dir.y;

        /* try a new position until if we are inside the border */
        while (x < 1 || y < 1 || x >= (d->w-1) || y >= (d->h-1)) {
            dir_change(&w->dir);
            x = w->x + w->dir.x;
            y = w->y + w->dir.y;
        }

        /* split a new worm segment */
        if (w->segment == NULL && rnd_coinflip(virility)) {
            worm_split(w);
        }

        /* move and eat the cell */
        w->x = x;
        w->y = y;
        worm_eat(d, w);

        if (rnd_coinflip(keel)) {
            dir_change(&w->dir);
        }
    }

    /* tick child segments */
    if (w->segment != NULL) {
        worm_tick(d, w->segment);
    }
}

void dg_worms(dg_dungeon d)
{
    dgx_seed_rng();

    struct worm* w = worm_create();
    w->x = d->w >> 1;
    w->y = d->h >> 1;

    worm_eat(d, w);
    worm_split(w);
    worm_split(w);
    worm_split(w);

    /* run the simulation for N steps */
    int steps = 0;

    while (++steps < d->generations) {
        worm_tick(d, w);

        /* check if all worms are dead before the generation is over */
        struct worm *wrm = w;
        int dead = w->dead;
        while (dead && wrm!= NULL) {
            dead = wrm->dead;
            wrm = wrm->segment;
        }
        if (dead) {
            worm_split(w);
        }
    }

    /* free use */
    worm_free(w);

    /* final step */
    if (d->step_fn != NULL) {
        d->step_fn(d, steps);
    }
}

