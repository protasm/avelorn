# North Island political atlas

## Current working borders

**The authoritative entry point is
[`pre-conquest/working-borders.json`](pre-conquest/working-borders.json).**

On September 6, 2026, the cell-level border proposal was adopted as working
version 1. The descriptor points to a versioned snapshot at
`pre-conquest/versions/001/package.json` and pins it by SHA-256. That package
contains the accepted country definitions, cell assignments, coordinate-label
mappings, derived border segments, and region checksums. Its validation report
is also pinned by the descriptor.

Future authoring and gameplay integration should resolve this descriptor rather
than selecting a draft by filename. The descriptor's relative paths are based
at its own directory; region and border paths inside the package are based at
the package directory. Adopted version directories are preserved. Future border
revisions should create a new version and explicitly update the descriptor.

The working borders are saved data; they are not yet connected to live gameplay.

## Adopted draft and scope

`pre-conquest/cell-draft-01/` refines the 53 pink regions in
`output/maps/north-island-pre-conquest-ownership-draft-05.json` into individual
256 m atlas squares. It covers the same connected North Island landmass as the
reference maps, including inland water but excluding separate offshore islands.
This was the review source for working version 1. Regenerating this development
draft does not alter the adopted versioned snapshot.

The physical Edition 1 JSON remains unchanged. All solid-color region ownership
from Draft 05 is preserved. The temporary country codes are:

| Code | Country |
| --- | --- |
| 1 | Western coast |
| 2 | Northern coast |
| 3 | Eastern coast |
| 4 | Southern coast |
| 5 | Central highlands |

Zero is not a country and is not assigned to any included square.

## Coordinate conventions

Data uses the existing atlas coordinates: X increases east, Y increases north,
and region indices are zero-based. Each region contains 64 × 64 squares.
For a square `(x,y)`:

```text
region_x = x // 64
region_y = y // 64
local_index = (y % 64) * 64 + (x % 64)
```

Map labels are a separate presentation convention. Columns A–Z map to native
regions 1–26; AA, BB, through ZZ map to 27–52. Display row is `77 - region_y`.
Thus HH12 means atlas region `(34,65)`; DD17 means `(30,60)`. Local indices do
not reverse with the displayed map rows. Both display mappings are in
`package.json`.

## Ownership files and lookup

The package referenced by the working descriptor lists 542 compressed region
files with their SHA-256 hashes,
country definitions, and references to the physical package and regional draft.
The layout mirrors the physical atlas: `regions/xNN/rNN_NN.json.gz`.

Each region records:

- `schema`: format version 1.
- `region_x`, `region_y`: native region address.
- `coverage_runs`: sorted `[start_local_index, count]` runs identifying included
  North Island squares. Runs may span local row boundaries.
- `default_nation`: the most common nation in that region.
- `overrides`: sorted `[local_index, nation_code]` entries for included squares
  belonging to another nation.

For lookup, first check that the local index belongs to a coverage run. An
absent index has no assignment in this package, even if a physical square
exists there on an offshore island. For an included index, return its override
if present, otherwise the region default. Coverage is necessary because some
physical region files also contain offshore land outside this draft's scope.

Loading a region can expand its runs and overrides into a cached 4,096-entry
lookup table. The game can later join this political record to its physical
square by native coordinates. Historical nationality should stay separate from
current political control and from a character's affiliation. No runtime
service or gameplay rules were added in this work.

`borders.json` is a derived rendering/export view of the same ownership records.
Each segment is `[x1,y1,x2,y2,nation_a,nation_b]`, using native **square-corner**
coordinates. Segments lie on shared cardinal square edges, have length one
square (256 m), and identify both neighboring countries. They describe internal
national boundaries, not coastlines. Nation codes are sorted and do not encode
which side of the directed segment belongs to which country; consult square
ownership for that. Square ownership in the adopted package is authoritative.

## How the first refinement was made

1. Validate physical source-region hashes against the frozen package manifest;
   select the North Island's four-neighbor connected spatial component.
2. Treat the solid regions as fixed ownership and pink regions as editable.
   Use the candidate country sets recorded in Draft 05, with one explicit
   correction: EE17 may include the northern country as well as the southern
   and central countries, allowing their river junction to sit inside the region.
3. Grow connected territory into the editable squares. Rivers and significant
   catchment paths resist crossing; steep relief and local elevation prominence
   provide additional guidance.
4. Route the shared border edges through these corridors, preferring river banks
   and prominent relief while retaining country connectivity and the fixed
   cores. A channel waypoint in HH12 ensures the requested northern extension
   participates in the border. Its exact native position is recorded in
   `validation.json`.
5. Export and independently decode the saved ownership files, then render maps
   directly from those assignments and exact shared edges.

Catchment continuity matters: a river can cross squares classified primarily as
wetland, forest, plateau, or gorge rather than open channel. River routing uses
the JSON catchment field as well as terrain codes. Detail maps show channels,
inland water, and flow paths with contributing area of at least 10,000 hectares
in blue. Such a flow-path square is not a claim that the whole 256 m footprint
is water. Each included square, including water, has one owner; river borders
follow square edges, not sub-square channel centerlines.

These borders originated as a terrain-guided political proposal. Locally prominent relief is a
heuristic rather than a surveyed watershed divide, and short connecting sections
may cross ordinary terrain. Three originally pink regions resolve to a single
country in version 1: Z20, NN18, and NN19. Future revisions use the versioning
procedure above.

## Validation and reproducibility

`validation.json` records coverage, nation areas, connectivity, retained core
assignments, candidate-set adjustment, lake and summit ownership, central
landlocked status, routing results, and serialized roundtrip checks. Every
included square receives exactly one country. Country connectivity includes
water and cliffs as territorial space; it does not certify walkable routes.

Rebuild using Python with NumPy and Pillow installed:

```sh
python3 atlas/political/tools/refine_borders.py
```

The defaults regenerate the development cell draft and its three images in
`output/maps/`. They do not update `working-borders.json` or the adopted snapshot,
nor overwrite Drafts 01–05, physical JSON, or Tiled maps. The script,
source-draft hash, and physical-package hash are recorded for reproducibility;
compressed outputs use a fixed gzip timestamp. Rendering currently uses macOS
Arial fonts. The country geometry does not depend on fonts.
