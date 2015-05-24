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

void dg_fill_rooms(dg_dungeon d)
{
    dg_each_room(d, fill_disconnected);
}

void dg_replace_all(dg_dungeon d, enum dg_cell_kind a, enum dg_cell_kind b)
{
    int i = d->w * d->h;

    while (i--) {
        if (d->cells[i].kind == a) {
            d->cells[i].kind = b;
        }
    }

    if (d->step_fn) {
        d->step_fn(d, d->ticks);
    }
}

void dg_fill(dg_dungeon d, enum dg_cell_kind fill)
{
    int i = d->w * d->h;
    while (i--) {
        d->cells[i].kind = fill;
    }

    d->ticks++;

    if (d->step_fn) {
        d->step_fn(d, d->ticks);
    }
}

void dg_fill_border(dg_dungeon d, int x, int y, int w, int h, enum dg_cell_kind fill)
{
    int xi, yi;
    for (yi=0; yi<h; yi++) {
        if (yi == 0 || yi == h - 1) {
            for (xi=0; xi<w; xi++) {
                dg_set(d, x+xi, y+yi, fill);
            }
        } else {
            dg_set(d, x, y+yi, fill);
            dg_set(d, x+w-1, y+yi, fill);
        }
    }
}

