# DUNGEN

The C dungeon generation toolkit

## Installation

Dungen is designed to be statically embedded.
`make libdungen.a` builds a standalone library you can link against.

## API

The API exposes an opaque dungeon structure that represents a grid and cells.
Different generation functions can be applied to a dungeon to create customizable effects.

#### Dungeon

* `dg_create` create a dungeon to work with
* `dg_free` cleanup memory used by a dungeon
* `dg_reset` reset dungeon

#### Working with cells

* `dg_clear` fast clear of all cells
* `dg_set` set an individual cell
* `dg_get` get an individual cell
* `dg_each` iterate over all cells
* `dg_each_room` iterate over room rects
* `dg_each_neighbor` iterate over neighbors of a cell

#### Dungeon generators

* `dg_worms` lots of random wormy passageways
* `dg_randomwalk` a random walk, great for generating connected caves
* `dg_chunky` many unconnected adjacent rooms
* `dg_smooth` remove artifacts
* `dg_blur` noise with bias
* `dg_fill` fill empty rooms
* `dg_replace_all` replace cell type a with cell type b
* `dg_noise` add random cells
* `dg_shrink` shrink random rooms
* `dg_voronoi` randomly filled voronoi cells
* `dg_maze` carve a maze

## Example

``` c
#include <stdlib.h>
#include "dungen.h"

int main()
{
    dg_dungeon d = dg_create(60, 40, NULL);
    dg_worms(d);
    dg_each(d, dg_print);
    dg_free(d);

    return 0;
}
```

## SDL Demo

Make will build an animated SDL demo by default.

![screenshot](https://raw.github.com/jdeseno/dungen/master/screenshot.png)
![screenshot](https://raw.github.com/jdeseno/dungen/master/screenshot-voronoi.png)
