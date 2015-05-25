#include <stdio.h>
#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

static int is_alive(enum dg_cell_kind k)
{
    return k == dg_cell_floor;
}

static void count_alive(dg_dungeon d, int x, int y, enum dg_cell_kind k, void *state)
{
    if (is_alive(k)) {
        int *alive = (int *)state;
        (*alive)++;
    }
}

static void cell_turn(dg_dungeon d, int x, int y, enum dg_cell_kind k)
{
    int alive = 0;
    dg_each_neighbor(d, x, y, &alive, count_alive);

    if (is_alive(k)) {
        if (alive < 2 || alive > 3) {
            dg_set(d, x, y, dg_cell_wall);
        }
    } else {
        if (alive == 3) {
            dg_set(d, x, y, dg_cell_floor);
        }
    }
}

void dg_life(dg_dungeon d)
{
    dg_each(d, cell_turn);
}

