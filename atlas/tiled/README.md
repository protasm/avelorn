# Avelorn Tiled world

This directory is the Tiled-facing view of Avelorn's frozen physical Edition 1
atlas. The physical terrain layer is generated from the tracked gzip JSON region
files and is locked in Tiled. Exact elevation and upstream-catchment values remain
in the source JSON rather than being duplicated as millions of Tiled objects.

`avelorn.world` arranges separate 64 by 64 region maps into a continuous world.
It contains all 542 atlas regions intersecting the connected North Island
landmass. The starting square at `x=113`, `y=3336` is the southern one of the
two westernmost Edition 1 squares at the island's western tip. This covers
1,740,269 connected North Island squares. The generated region files contain
1,740,951 populated squares in total because boundary maps retain small offshore
islands that share those regions.

`maps/r01_52.tmj` contains the starting square. Its western and southern
neighbors are omitted Edition 1 ocean squares, leaving north and east as the
two exits from the tip.

A reference corridor through the complete World runs:

```text
32,61 -> 31,61 -> 31,62 -> 30,62 -> 30,63 -> 29,63 -> 29,64 -> 28,64
```

`maps/r28_64.tmj` is the coastal map at the end of that corridor; its 1,043
omitted Edition 1 ocean squares appear as empty tiles.

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

The large inland lake occupies parts of Regions
34-35 by 59-61. The World includes a one-region collar around the complete
lake: every region in the 4 by 5 block from `33,58` through `36,62`. Region
`33,61` joins that block to Region `32,61`.

The large stratovolcano beyond the lake has its 2,651 m summit in Region
`37,58`. Its connected terrain above 1,200 m lies within Regions 36-38 by
57-59, so the World includes that complete 3 by 3 block. It overlaps the lake
collar at Regions `36,58` and `36,59`, keeping the starting region, lake, and
mountain area continuously connected.

## Rebuild or add a region

From the repository root:

```sh
python3 atlas/tiled/tools/build_tiled_region.py --north-island
```

This finds the complete connected landmass containing the starting square and
rebuilds every region it intersects. To rebuild or add just one populated
region instead:

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
