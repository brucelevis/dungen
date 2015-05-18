#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

#define SPLIT_LEN 12

struct rect_t {
    struct rect rect;
    struct rect_t *left, *right;
};

static struct rect_t *create_rect_t()
{
    struct rect_t *t;

    if ((t = malloc(sizeof(struct rect_t))) == NULL) {
        dg_err("create_rect_t: out of memory");
    }

    t->left = t->right = NULL;

    return t;
}

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

static void split(struct rect_t *t)
{
    int do_split = 0;
    int split_dir;

    if (t->rect.h >= SPLIT_LEN && t->rect.w >= SPLIT_LEN) {
        do_split = 1;
        split_dir = rnd_coinflip(0);

        // permit less uniform cells
        if ((t->rect.h <= SPLIT_LEN * 1.6 && t->rect.w <= SPLIT_LEN * 1.6) && rnd_coinflip(1)) {
            do_split = 0;
        } else if ((t->rect.h <= SPLIT_LEN * 1.4 && t->rect.w <= SPLIT_LEN * 1.4) && rnd_coinflip(0)) {
            do_split = 0;
        }
    } else if (t->rect.h >= SPLIT_LEN) {
        do_split = 1;
        split_dir = 0;
    } else if (t->rect.w >= SPLIT_LEN) {
        do_split = 1;
        split_dir = 1;
    }

    if (do_split) {
        t->left = create_rect_t();
        t->right = create_rect_t();

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
        if (t->left != NULL) carve_tree(d, t->left);
        if (t->right != NULL) carve_tree(d, t->right);
    }
}

static void free_tree(struct rect_t* t)
{
    if (t->left != NULL) free_tree(t->left);
    if (t->right != NULL) free_tree(t->right);

    free(t);
}

static void copy_rooms(dg_dungeon d, struct rect_t *t)
{
    if (t->left == NULL && t->right == NULL) {
        add_room(d, t->rect);
    } else {
        if (t->left != NULL) copy_rooms(d, t->left);
        if (t->right != NULL) copy_rooms(d, t->right);
    }
}

void dg_chunky(dg_dungeon d)
{
    seed_rng();

    struct rect_t *tree;

    if ((tree = malloc(sizeof(struct rect_t))) == NULL) {
        dg_err("dg_chunky: out of memory");
    }

    tree->left = tree->right = NULL;
    tree->rect.x = tree->rect.y = 0;
    tree->rect.w = d->w;
    tree->rect.h = d->h;

    split(tree);
    carve_tree(d, tree);
    copy_rooms(d, tree);
    free_tree(tree);
}

