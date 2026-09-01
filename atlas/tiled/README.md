# Avelorn Tiled world

This directory is the Tiled-facing view of Avelorn's frozen physical Edition 1
atlas. The physical terrain layer is generated from the tracked gzip JSON region
files and is locked in Tiled. Exact elevation and upstream-catchment values remain
in the source JSON rather than being duplicated as millions of Tiled objects.

`avelorn.world` arranges separate 64 by 64 region maps into a continuous world.
The represented regions currently form the shortest straight corridor from the
starting region to the northwestern ocean:

```text
32,61 -> 31,61 -> 31,62 -> 30,62 -> 30,63 -> 29,63 -> 29,64 -> 28,64
```

`maps/r32_61.tmj` contains the starting square at atlas coordinates
`x=2080`, `y=3936`. `maps/r28_64.tmj` is the coastal map; its 1,043 omitted
Edition 1 ocean squares appear as empty tiles.

From Region 28,64, ten additional maps trace the connected coastline north:

```text
28,65 -> 29,65 -> 29,66 -> 30,66 -> 30,67 ->
31,67 -> 31,68 -> 31,69 -> 32,69 -> 32,70
```

The next ten maps continue along that coastline from Region 32,70:

```text
32,71 -> 31,71 -> 31,72 -> 30,72 -> 30,73 ->
30,74 -> 30,75 -> 31,75 -> 31,76 -> 32,76
```

The large inland lake beyond the starting region occupies parts of Regions
34-35 by 59-61. The World includes a one-region collar around the complete
lake: every region in the 4 by 5 block from `33,58` through `36,62`. Region
`33,61` joins that block directly to the starting Region `32,61`.

The large stratovolcano beyond the lake has its 2,651 m summit in Region
`37,58`. Its connected terrain above 1,200 m lies within Regions 36-38 by
57-59, so the World includes that complete 3 by 3 block. It overlaps the lake
collar at Regions `36,58` and `36,59`, keeping the starting region, lake, and
mountain area continuously connected.

## Rebuild or add a region

From the repository root:

```sh
python3 atlas/tiled/tools/build_tiled_region.py 32 61
```

Running the tool for another populated region writes its `.tmj` map and rebuilds
the World file to include every generated map under `maps/`.

## Coordinates

The Edition 1 JSON origin is southwest and its `y` coordinate increases north.
Tiled rows increase downward. The converter therefore reverses the 64 local rows
inside each region and places region `y=79` at the top of the World.

## Generated layers

- `Physical Terrain (Edition 1)`: locked tile layer generated from terrain codes.
- `Areas`: empty object layer reserved for later local-area footprints.
- `Entrances`: empty object layer reserved for later atlas-to-area links.

The tileset is intentionally schematic. It gives each Edition 1 terrain code a
stable colored tile and retains the terrain identifier and code as tile
properties; it is not final map art.
