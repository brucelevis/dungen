#include <stdlib.h>
#include "dungen.h"
#include "lib.h"

#define SIZE 4

struct v_centroid {
    struct point point;
    int count;
    int capacity;
    struct point *points;
};

static void add_point(struct v_centroid *vc, int x, int y)
{
    if (vc->count >= vc->capacity) {
        struct point *tmp = realloc(vc->points, sizeof(struct point) * vc->capacity * 2);

        if (tmp == NULL) {
            dg_panic("voronoi: out of memory");
        } else {
            vc->points = tmp;
            vc->capacity *= 2;
        }
    }

    struct point *p = &vc->points[vc->count++];
    p->x = x;
    p->y = y;
}

static int cmp_centroid(const void *a, const void *b)
{
    struct point *pa = &((struct v_centroid *)a)->point;
    struct point *pb = &((struct v_centroid *)b)->point;

    if (pa->x > pb->x) {
        return 1;
    } else if (pa->x == pb->x) {
        return (pa->y > pb->y ? 1 : (pa->y == pb->y ? 0 : -1));
    } else {
        return -1;
    }
}

void dg_voronoi(dg_dungeon d)
{
    seed_rng();

    /* create centroids */
    int count = d->w * d->h / (SIZE * SIZE);
    count -= count % 2;
    size_t size_v_centroid = sizeof(struct v_centroid);
    struct v_centroid *centroids = malloc(size_v_centroid * count);
    int i;

    /* initialize */
    for (i=0; i<count; i++) {
        struct v_centroid *vc = &centroids[i];

        vc->count = 0;
        vc->capacity = 4;
        vc->points = malloc(sizeof(struct point) * vc->capacity);
        vc->point.x = rnd_range(0, d->w);
        vc->point.y = rnd_range(0, d->h);
    }

    /* sort centroids */
    qsort(centroids, count, size_v_centroid, cmp_centroid);

    /* - assign nearest cells to centroids
       - copy each pixel */
    for (int y=0; y<d->h; y++) {
        for (int x=0; x<d->w; x++) {

            struct v_centroid *nearest = centroids;

            for (i=0; i<count; i++) {
                struct v_centroid *vc = &centroids[i];

                int d1 = abs(nearest->point.x - x) + abs(nearest->point.y - y);
                int d2 = abs(vc->point.x - x) + abs(vc->point.y - y);

                if (d1 > d2) {
                    nearest = vc;
                    /* XXX break? */
                }
            }

            add_point(nearest, x, y);
        }
    }

    /* carve & free memory */
    int flip = 0;
    enum dg_cell_kind kind;
    struct v_centroid *vc;

    for (i=0; i<count; i++) {
        vc = &centroids[i];
        kind = (flip ^= 1) ? dg_cell_floor : dg_cell_wall;

        for (int j=0; j<vc->count; j++) {
            dg_set(d, vc->points[j].x, vc->points[j].y, kind);
        }

        /* free points */
        free(vc->points);
    }

    /* important: points are freed above */
    free(centroids);
}
