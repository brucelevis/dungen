#ifndef __DUNGEN_H__
#define __DUNGEN_H__

enum dg_cell_kind {
    dg_cell_stone,
    dg_cell_wall,
    dg_cell_floor
};

struct dgx_dungeon;
typedef struct dgx_dungeon * dg_dungeon;
typedef void (*dg_render_step)(dg_dungeon d, int step);
typedef void (*dg_each_cell)(dg_dungeon d, int x, int y, enum dg_cell_kind k);

dg_dungeon dg_create(int width, int height, dg_render_step step_fn);
void dg_each(dg_dungeon d, dg_each_cell fn);
void dg_print(dg_dungeon d, int x, int y, enum dg_cell_kind kind);
void dg_free(dg_dungeon d);
void dg_worms(dg_dungeon d);
void dg_smooth(dg_dungeon d);
void dg_drunkenwalk(dg_dungeon d);

#endif
