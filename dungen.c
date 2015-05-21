#include <stdio.h>
#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

dg_dungeon dg_create(int width, int height, dg_render_step step_fn)
{
    struct dgx_dungeon* d;

    if ((d = malloc(sizeof(struct dgx_dungeon))) == NULL) {
        dg_err("dg_create: out of memory");
    }

    d->w = width;
    d->h = height;
    d->ticks = 0;
    d->generations = ((d->w + d->h) / 2) * 10;
    d->rooms = NULL;

    d->cells = malloc(sizeof(struct cell) * d->w * d->h);
    if (d->cells == NULL) {
        dg_err("dg_create: out of memory");
    }

    d->step_fn = step_fn;

    int i = 0;
    for (int y=0; y < d->h; y++) {
        for (int x=0; x < d->w; x++) {
            struct cell *cell = &d->cells[i++];
            cell->x = x;
            cell->y = y;
            cell->kind = dg_cell_stone;
        }
    }

    return d;
}

void dg_free(dg_dungeon d)
{
    free(d);
}

void dg_reset(dg_dungeon d)
{
    d->ticks = 0;

    /* reset rooms */
    struct rect_l *rn, *rl=d->rooms;

    while (rl != NULL) {
        rn = rl;
        rl = rl->next;
        free(rn);
    }

    d->rooms = NULL;

    dg_clear(d);
}

void dg_clear(dg_dungeon d)
{
    int i = d->w * d->h;
    while (i--) {
        d->cells[i].kind = dg_cell_stone;
    }

    d->ticks++;

    if (d->step_fn) {
        d->step_fn(d, d->ticks);
    }
}

void dg_set(dg_dungeon d, int x, int y, enum dg_cell_kind kind)
{
    d->ticks++;

    CELL_AT(d, x, y).kind = kind;

    if (d->step_fn) {
        d->step_fn(d, d->ticks);
    }
}

enum dg_cell_kind dg_get(dg_dungeon d, int x, int y)
{
    return CELL_AT(d, x, y).kind;
}

void dg_each(dg_dungeon d, dg_each_cell fn)
{
    for (int y=0; y<d->h; y++) {
        for (int x=0; x<d->w; x++) {
            fn(d, x, y, dg_get(d, x, y));
        }
    }
}

void dg_each_room(dg_dungeon d, dg_each_rect fn)
{
    struct rect_l *r = d->rooms;
    while (r != NULL) {
        fn(d, r->rect.x, r->rect.y, r->rect.w, r->rect.h);
        r = r->next;
    }
}

void dg_each_neighbor(dg_dungeon d, int x, int y, void *persist, dg_each_neighbor_cb fn)
{
    /* top */
    if (y > 0) {
        if (x > 0) {
            fn(d, x-1, y-1, dg_get(d, x-1, y-1), persist);
        }

        fn(d, x+1, y-1, dg_get(d, x, y-1), persist);

        if (x < (d->w - 1)) {
            fn(d, x+1, y-1, dg_get(d, x+1, y-1), persist);
        }
    }

    /* middle */
    if (x > 0) {
        fn(d, x-1, y, dg_get(d, x-1, y), persist);
    }
    if (x < (d->w - 1)) {
        fn(d, x+1, y, dg_get(d, x+1, y), persist);
    }

    /* bottom */
    if (y < d->h) {
        if (x > 0) {
            fn(d, x-1, y+1, dg_get(d, x+1, y+1), persist);
        }

        fn(d, x, y+1, dg_get(d, x, y+1), persist);

        if (x < (d->w - 1)) {
            fn(d, x+1, y+1, dg_get(d, x+1, y+1), persist);
        }
    }
}

void dg_print(const dg_dungeon d, int x, int y, enum dg_cell_kind kind)
{
    switch (kind) {
        case dg_cell_stone:
            putchar(' ');
            break;
        case dg_cell_wall:
            putchar('#');
            break;
        case dg_cell_floor:
            putchar('.');
            break;
    }
    if (x == (d->w - 1)) {
        putchar('\n');
    }
}

void add_room(dg_dungeon d, struct rect room)
{
    if (d->rooms == NULL) {
        d->rooms = malloc(sizeof(struct rect_l));

        if (d->rooms == NULL) {
            dg_err("add_room: out of memory");
        }

        d->rooms->rect = room;
        d->rooms->next = NULL;
    } else {
        struct rect_l *rl = d->rooms;
        while (rl->next != NULL) {
            rl = rl->next;
        }
        rl->next = malloc(sizeof(struct rect_l));

        if (rl->next == NULL) {
            dg_err("add_room: out of memory");
        }

        rl->next->rect = room;
        rl->next->next = NULL;
    }
}

static int rect_eq(const struct rect *a, const struct rect *b)
{
    return a->x == b->x && a->y == b->y && a->w == b->w && a->h == b->h;
}

void remove_room(dg_dungeon d, struct rect room)
{
    struct rect_l *rl = d->rooms;

    while (rl != NULL) {
        if (rect_eq(&rl->rect, &room)) {
            if (rl->next) {
                struct rect_l *tmp = rl;
                rl = rl->next;
                free(tmp);
                break;
            }
        }

        rl = rl->next;
    }
}

void carve_room(dg_dungeon d, struct rect rect)
{
    enum dg_cell_kind kind;

    for (int y=0; y<rect.h; y++) {
        for (int x=0; x<rect.w; x++) {
            if (y == 0 || y == (rect.h - 1) || x == 0 || x == (rect.w - 1)) {
                kind = dg_cell_wall;
            } else {
                kind = dg_cell_floor;
            }

            dg_set(d, rect.x+x, rect.y+y, kind);
        }
    }
}

void dg_err(const char *msg)
{
#ifdef DG_ERR
    (void)DG_ERR(msg);
#else
    fprintf(stderr, "[dungen] %s\n", msg);
    exit(1);
#endif
}

