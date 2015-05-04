#include <stdlib.h>
#include <time.h>
#include "dungen.h"
#include "lib.h"

void seed_rng()
{
    srand(time(NULL));
}

int rnd_range(int min, int max)
{
    return min + rand() % (max - min);
}

int rnd_coinflip(int n)
{
    int heads;

    do {
        heads = rand() % 2 > 0;
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

struct point dir_rnd()
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

