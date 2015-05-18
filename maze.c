#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

// based on Eller's algorithm, creates decayed looking mazes with open spaces
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

        // draw & copy row
        for (int x=0; x<d->w; x++) {
            dg_set(d, x, y, l[x]);
            p[x] = l[x];
        }

        // adjust row
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

void dg_maze(dg_dungeon d)
{
    seed_rng();
    spacey_eller(d);
}
