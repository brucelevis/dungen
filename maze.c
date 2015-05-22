#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

/* (loosely) based on Eller's algorithm, creates decayed looking mazes with open spaces */
static void spacey_eller(dg_dungeon d)
{
    enum dg_cell_kind *l, *p;

    if ((l = malloc(sizeof(enum dg_cell_kind) * d->w)) == NULL) {
        dg_err("dg_maze: out of memory");
    }
    if ((p = malloc(sizeof(enum dg_cell_kind) * d->w)) == NULL) {
        dg_err("dg_maze: out of memory");
    }

    for (int i=0; i<d->w; i++) {
        p[i] = l[i] = rnd_coinflip(0) ? dg_cell_floor : dg_cell_wall;
    }

    for (int y=0; y<d->h; y++) {

        /* draw & copy row */
        for (int x=0; x<d->w; x++) {
            dg_set(d, x, y, l[x]);
            p[x] = l[x];
        }

        /* adjust row */
        for (int i=0; i<d->w; i++) {

            l[i] = dg_cell_floor;

            if (rnd_coinflip(0)) {
                int rchk = i < (d->w - 1) ? p[i+1] == dg_cell_floor : 1;
                int lchk = i > 0 ? p[i-1] == dg_cell_floor : 1;

                if (l[i] == dg_cell_wall || (lchk && rchk)) {
                    l[i] = dg_cell_wall;
                }
            }
        }

    }

    free(l);
    free(p);
}

void dg_forest(dg_dungeon d)
{
    seed_rng();
    spacey_eller(d);
}

static void move(int *x, int *y, int dir)
{
    switch (dir) {
        case 0: --(*y); break;
        case 1: (*x)++; break;
        case 2: (*y)++; break;
        case 3: --(*x); break;
        default: dg_err("move: bad direction");
    }
}

static int dir_allowed(dg_dungeon d, int x, int y, int dir)
{
    move(&x, &y, dir);
    move(&x, &y, dir);

    if (x<1 || y<1 || x>d->w-2 || y>d->h-2 || dg_get(d, x, y) == dg_cell_floor) {
        return 0;
    }

    return 1;
}

static int odd(int n)
{
    if (n > 1 && n % 2 == 0) n--;
    return n;
}

void dg_maze(dg_dungeon d)
{
    seed_rng();

    int i, choices, x, y, a, dir, dirs[4];
    struct point *open;

    if ((open = malloc(sizeof(struct point) * d->w * d->h)) == NULL) {
        dg_err("dg_maze: out of memory");
    }

    i = 0;
    open[i].x = odd(rnd_range(3, d->w-2));
    open[i].y = odd(rnd_range(3, d->h-2));

    while (i >= 0) {
        x = open[i].x;
        y = open[i].y;
        dg_set(d, x, y, dg_cell_floor);

        choices = 0;
        for (a=0; a<4; a++) {
            if (dir_allowed(d, x, y, a)) {
                dirs[choices++] = a;
            }
        }

        /* select a random open cell */
        if (choices > 0) {
            dir = dirs[rnd_range(0, choices)];

            /* move twice in the same direction, setting the middle cell */
            move(&x, &y, dir);
            dg_set(d, x, y, dg_cell_floor);
            move(&x, &y, dir);

            i++;
            open[i].x = x;
            open[i].y = y;
        } else {
            i--;
        }
    }

    free(open);
}
