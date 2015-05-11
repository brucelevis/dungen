# DUNGEN

C dungeon generation library designed for easy static embedding.

## Installation

`make libdungen.a` builds a static library you can link against.

## Usage

The API exposes an opaque dungeon structure that represents a grid and cells.
Different generation functions can be applied to a dungeon to create customizable effects.

## Example

``` c
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
