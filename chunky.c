#include <stdlib.h>
#include <stdio.h>
#include "dungen.h"
#include "lib.h"

#define SPLIT_LEN 10

struct rect_t {
    struct rect rect;
    struct rect_t *left, *right;
};

static int rnd_midpoint(int v)
{
    int split = v / 2;
    int unit = v / SPLIT_LEN;
    if (unit == 0) { unit = 1; }

    return rnd_range(split - unit, split + unit * 2);
}

// split_rect splits with overlap between the new a & b
static void split_rect(int split_width, struct rect *rect, struct rect *a, struct rect *b)
{
    if (split_width) {
        int m = rnd_midpoint(rect->w);

        a->x = rect->x;
        a->y = rect->y;
        a->w = rect->w - m;
        a->h = rect->h;

        b->x = rect->x + (a->w - 1);
        b->y = rect->y;
        b->w = rect->w - (a->w - 1);
        b->h = rect->h;
    } else {
        int m = rnd_midpoint(rect->h);

        a->x = rect->x;
        a->y = rect->y;
        a->w = rect->w;
        a->h = rect->h - m;

        b->x = rect->x;
        b->y = rect->y + (a->h - 1);
        b->w = rect->w;
        b->h = rect->h - (a->h - 1);
    }
}

static void carve_room(dg_dungeon d, struct rect rect)
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

static void split(struct rect_t *t)
{
    int do_split = 0;
    int split_dir;

    if (t->rect.h >= SPLIT_LEN && t->rect.w >= SPLIT_LEN) {
        do_split = 1;
        split_dir = rnd_coinflip(0);
    } else if (t->rect.h >= SPLIT_LEN) {
        do_split = 1;
        split_dir = 0;
    } else if (t->rect.w >= SPLIT_LEN) {
        do_split = 1;
        split_dir = 1;
    }

    if (do_split) {
        t->left = malloc(sizeof(struct rect_t));
        t->left->left = t->left->right = NULL;

        t->right = malloc(sizeof(struct rect_t));
        t->right->left = t->right->right = NULL;

        split_rect(split_dir, &t->rect, &t->left->rect, &t->right->rect);

        split(t->left);
        split(t->right);
    }
}

static void carve_tree(dg_dungeon d, struct rect_t *t)
{

    if (t->left == NULL && t->right == NULL) {
        carve_room(d, t->rect);
    } else {

        if (t->left != NULL) {
            carve_tree(d, t->left);
        }

        if (t->right != NULL) {
            carve_tree(d, t->right);
        }
    }
}

static void free_tree(struct rect_t* t)
{
    if (t->left != NULL) {
        free_tree(t->left);
    }
    if (t->right != NULL) {
        free_tree(t->right);
    }

    free(t);
}

void dg_chunky(dg_dungeon d)
{
    seed_rng();

    struct rect_t *tree = malloc(sizeof(struct rect_t));
    tree->left = tree->right = NULL;
    tree->rect.x = tree->rect.y = 0;
    tree->rect.w = d->w;
    tree->rect.h = d->h;

    split(tree);
    carve_tree(d, tree);

    free_tree(tree);
}
