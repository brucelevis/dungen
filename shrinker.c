#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

#define MIN_SIZE 6

void rnd_shrink(dg_dungeon d, int x, int y, int w, int h)
{
    int extra = rnd_range(0, 2);
    do {
        if (w > MIN_SIZE && h > MIN_SIZE && rnd_coinflip(0)) {

            struct rect_l *rl = d->rooms;

            while (rl != NULL) {

                if (rl->rect.x == x && rl->rect.y == y && rl->rect.w == w && rl->rect.h == h) {

                    for (int yi=0; yi < (h-2); yi++) {
                        for (int xi=0; xi < (w-2); xi++) {
                            dg_set(d, x+xi, y+yi, dg_cell_wall);
                        }
                    }

                    rl->rect.x++;
                    rl->rect.y++;
                    rl->rect.w -= 2;
                    rl->rect.h -= 2;

                    carve_room(d, rl->rect);

                    break;
                }

                rl = rl->next;
            }
        }
    } while (extra--);
}

void dg_shrink(dg_dungeon d)
{
    dg_each_room(d, rnd_shrink);
}
