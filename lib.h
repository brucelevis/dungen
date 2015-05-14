#ifndef __LIB_H__
#define __LIB_H__

#define CELL_AT(d, x, y) (d->cells[x + y * d->w])

struct point {
    int x;
    int y;
};

struct rect {
    int x, y, w, h;
};

struct rect_l {
    struct rect rect;
    struct rect_l *next;
};

struct cell {
    int x;
    int y;
    enum dg_cell_kind kind;
};

struct dgx_dungeon {
    int w;
    int h;
    int ticks;
    int generations;
    dg_render_step step_fn;
    struct rect_l* rooms;
    struct cell *cells;
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
void draw_line(dg_dungeon d, int x1, int y1, int x2, int y2);
void add_room(dg_dungeon d, struct rect room);

#endif
