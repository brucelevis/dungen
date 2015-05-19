#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

void dg_smooth(dg_dungeon d)
{
    /* TODO store floor cells in list; run this once then run smoothing functions */
    for (int y=0; y<d->h; y++) {
        for (int x=0; x<d->w; x++) {

            struct cell cell = CELL_AT(d, x, y);

            /* remove islands; cells completely surrounded by floors */
            if (cell.kind != dg_cell_floor) {
                int floors = 0;

                for (int ni=0; ni<8; ni++) {
                    int nx = x + DIRS[ni * 2];
                    int ny = y + DIRS[ni * 2 + 1];

                    if (nx >= 0 && ny >= 0 && nx < d->w && ny < d->h) {
                        struct cell ncell = CELL_AT(d, nx, ny);

                        if (ncell.kind == dg_cell_floor) {
                            floors++;
                        } else {
                            break; /* every cell must be a floor */
                        }
                    }
                }

                if (floors == 8) {
                    cell.kind = dg_cell_floor;
                }
            }

        }
    }
}
