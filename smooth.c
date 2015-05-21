#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

struct counts {
    int floors;
    int walls;
    int stones;
};

static enum dg_cell_kind count_cell_kind(struct counts *counts)
{
    if (counts->walls > counts->floors) {
        if (counts->walls > counts->stones) {
            return dg_cell_wall;
        } else {
            return dg_cell_stone;
        }
    } else {
        if (counts->floors > counts->stones) {
            return dg_cell_floor;
        } else {
            return dg_cell_stone;
        }
    }
}

static void collect_counts(dg_dungeon d, int x, int y, enum dg_cell_kind k, void *persist)
{
    struct counts *c = (struct counts *)persist;

    switch (k) {
        case dg_cell_floor:
            c->floors++;
            break;
        case dg_cell_wall:
            c->walls++;
            break;
        case dg_cell_stone:
            c->stones++;
            break;
    }
}

static void noise_smooth(dg_dungeon d, int x, int y, enum dg_cell_kind kind)
{
    /* walls, floors, stones */
    struct counts counts;
    counts.floors = counts.walls = counts.stones = 0;

    /* count the current cell */
    collect_counts(d, x, y, kind, &counts);

    /* add a random chance the cell will change
       chance should be stronger when cell is less certain
       edge cells should have a lower chance of change */
    int chance = (y > 0 ? 1 : 0)
               + (y < (d->h - 1) ? 1 : 0)
               - (x > 0 ? 0 : 1)
               - (x < (d->w - 1) ? 0 : 1);

    while (chance--) {
        collect_counts(d, x, y, (enum dg_cell_kind)rnd_range(0, 3), &counts);
    }

    dg_each_neighbor(d, x, y, &counts, collect_counts);

    /* set the cell to the highest count */
    dg_set(d, x, y, count_cell_kind(&counts));
}

/* blur is close to smooth, it's smooth with bias for existing cells */
static void blur(dg_dungeon d, int x, int y, enum dg_cell_kind kind)
{
    /* walls, floors, stones */
    struct counts counts;
    counts.floors = counts.walls = counts.stones = 0;

    /* the current cell creates a bias */
    collect_counts(d, x, y, kind, &counts);

    /* add a random chance the cell will change */
    int chance = (y > 0 ? 1 : 0)
               + (y < (d->h - 1) ? 1 : 0)
               - (x > 0 ? 0 : 1)
               - (x < (d->w - 1) ? 0 : 1);

    while (chance--) {
        collect_counts(d, x, y, (enum dg_cell_kind)rnd_range(0, 3), &counts);
    }

    /* set the cell to the highest count */
    dg_set(d, x, y, count_cell_kind(&counts));
}

void dg_smooth(dg_dungeon d)
{
    dg_each(d, noise_smooth);
}

void dg_blur(dg_dungeon d)
{
    dg_each(d, blur);
}
