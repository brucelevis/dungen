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

