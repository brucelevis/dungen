#include "dungen.h"
#include "lib.h"

void rnd_set(dg_dungeon d, int x, int y, enum dg_cell_kind kind)
{
    enum dg_cell_kind k =
        rnd_coinflip(0) ?
        kind :
        (kind == dg_cell_wall ? dg_cell_floor : dg_cell_wall);

    dg_set(d, x, y, k);
}

void dg_noise(dg_dungeon d)
{
    dg_each(d, rnd_set);
}
