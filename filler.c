#include "dungen.h"
#include "lib.h"

static void fill_disconnected(dg_dungeon d, int x, int y, int w, int h)
{
    int connected = 0;

    for (int yi=0; yi < h; yi++) {
        if (yi == 0 || yi == (h - 1)) {
            for (int xi=1; xi < w-1; xi++) {
                if (dg_get(d, x+xi, y+yi) == dg_cell_floor) {
                    connected = 1;
                    goto fill;
                }
            }
        } else {
            if (dg_get(d, x, y+yi) == dg_cell_floor || dg_get(d, x+w-1, y+yi) == dg_cell_floor) {
                connected = 1;
                goto fill;
            }
        }
    }

fill:
    if (!connected) {
        x += 1;
        y += 1;
        for (int yi=0; yi < (h-2); yi++) {
            for (int xi=0; xi < (w-2); xi++) {
                dg_set(d, x+xi, y+yi, dg_cell_wall);
            }
        }

        struct rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;

        remove_room(d, rect);
    }
}

void dg_fill(dg_dungeon d)
{
    dg_each_room(d, fill_disconnected);
}
