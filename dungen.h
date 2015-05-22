#ifndef __DUNGEN_H__
#define __DUNGEN_H__

#ifdef __cplusplus
extern "C" {
#endif

enum dg_cell_kind {
    dg_cell_stone = 0,
    dg_cell_wall,
    dg_cell_floor
};

struct dgx_dungeon;
typedef struct dgx_dungeon * dg_dungeon;
typedef void (*dg_render_step)(dg_dungeon d, int step);
typedef void (*dg_each_cell)(dg_dungeon d, int x, int y, enum dg_cell_kind k);
typedef void (*dg_each_rect)(dg_dungeon d, int x, int y, int w, int h);
typedef void (*dg_each_neighbor_cb)(dg_dungeon d, int x, int y, enum dg_cell_kind k, void *persist);

dg_dungeon dg_create(int width, int height, dg_render_step step_fn);
void dg_err(const char *msg);
void dg_reset(dg_dungeon d);
void dg_clear(dg_dungeon d);
void dg_each(dg_dungeon d, dg_each_cell fn);
void dg_set(dg_dungeon d, int x, int y, enum dg_cell_kind kind);
enum dg_cell_kind dg_get(dg_dungeon d, int x, int y);
void dg_each_room(dg_dungeon d, dg_each_rect fn);
void dg_each_neighbor(dg_dungeon d, int x, int y, void *persist, dg_each_neighbor_cb fn);
void dg_print(dg_dungeon d, int x, int y, enum dg_cell_kind kind);
void dg_free(dg_dungeon d);
void dg_worms(dg_dungeon d);
void dg_smooth(dg_dungeon d);
void dg_randomwalk(dg_dungeon d);
void dg_chunky(dg_dungeon d);
void dg_fill(dg_dungeon d);
void dg_replace_all(dg_dungeon d, enum dg_cell_kind a, enum dg_cell_kind b);
void dg_blur(dg_dungeon d);
void dg_noise(dg_dungeon d);
void dg_shrink(dg_dungeon d);
void dg_voronoi(dg_dungeon d);
void dg_forest(dg_dungeon d);
void dg_maze(dg_dungeon d);

#ifdef __cplusplus
}
#endif

#endif
