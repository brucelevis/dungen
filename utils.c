#include <stdlib.h>
#include <time.h>
#include "dungen.h"
#include "lib.h"

#ifdef __APPLE__
#define random arc4random
#endif

#define RND(x) (int)(random() % (long)(x))

void dgx_seed_rng(void)
{
    static int init = 0;
    if (!init) {
        srandom(time(NULL));
        init = 1;
    }
}

int dgx_rnd_range(int min, int max)
{
    return min + RND(max - min);
}

int rnd_coinflip(int n)
{
    int heads;

    do {
        heads = RND(2) > 0;
    } while (heads && n--);

    return heads;
}

void dir_change(struct point *p)
{
    if (p->x == 0) {
        p->x = rnd_coinflip(0) ? -1 : 1;
        p->y = 0;
    } else {
        p->x = 0;
        p->y = rnd_coinflip(0) ? -1 : 1;
    }
}

struct point dir_rnd(void)
{
    struct point p;

    if (rnd_coinflip(0)) {
        p.y = rnd_coinflip(0) ? -1 : 1;
        p.x = 0;
    } else {
        p.x = rnd_coinflip(0) ? -1 : 1;
        p.y = 0;
    }

    return p;
}

void draw_line(dg_dungeon d, int x1, int y1, int x2, int y2, enum dg_cell_kind fill)
{

    int xd =  abs(x2 - x1),
        yd = -abs(y2 - y1),
        x_step = x1 < x2 ? 1 : -1,
        y_step = y1 < y2 ? 1 : -1,
        error = xd + yd;

    dg_set(d, x1, y1, fill);

    while (x1 != x2 || y1 != y2) {
        if (2 * error - yd > xd - 2 * error) {
            error += yd;
            x1 += x_step;
        } else {
            error += xd;
            y1 += y_step;
        }

        dg_set(d, x1, y1, fill);
    }
}

