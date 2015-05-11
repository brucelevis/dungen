#ifndef __LIB_H__
#define __LIB_H__

#define CELL_AT(d, x, y) (d->cells[x + y * d->w])

struct point {
    int x;
    int y;
};

struct cell {
    int x;
    int y;
    enum dg_cell_kind kind;
};

struct dgx_dungeon {
    int w;
    int h;
    int generations;
    dg_render_step step_fn;
    struct cell *cells;
};

struct worm {
    int x;
    int y;
    struct point dir;
    int ticks;
    int dead;
    int segment_depth;
    struct worm* segment;
};

static const int DIRS[] =
{
    -1, -1, 0, -1, 1, -1,
    -1,  0,        1,  0,
    -1,  1, 0, 1,  1,  1
};

void seed_rng();
int rnd_range(int min, int max);
int rnd_coinflip(int n);
void dir_change(struct point *p);
struct point dir_rnd();

#endif
