# Unnamed Physical World: Coordinate Contract

## Working canonical orientation

- Fictional north is the top of the map.
- The retained land geometry is permanently oriented 75 degrees
  counterclockwise from its Earth-referenced source orientation.
- The QGIS canvas rotation is 0 degrees.
- Positive x is fictional east and positive y is fictional north.

## True-metre source frame

All distance-bearing physical geometry is first projected into NZGD2000 / New
Zealand Transverse Mercator 2000 (NZTM2000, EPSG:2193). The 75-degree affine
rotation is then applied in that true-metre plane. A rigid rotation preserves
lengths, areas, angles, and topology.

For an Earth-referenced NZTM2000 coordinate `(xe, yn)`, let `(px, py)` be the
declared pivot and let `a = 75 degrees`. The fictional-world coordinate is:

```text
x = px + cos(a) * (xe - px) - sin(a) * (yn - py)
y = py + sin(a) * (xe - px) + cos(a) * (yn - py)
```

The inverse uses `-75 degrees`. This is a rotation of the physical data, not a
QGIS canvas rotation. Bearings in the fictional world are measured against the
resulting axes: increasing `x` is east, increasing `y` is north, and a project
view rotation of zero places fictional north at the top of the screen.

QGIS uses EPSG:3857 only as a compatible storage envelope for the resulting
fictional Cartesian coordinates. After rotation those coordinates are not Web
Mercator and are not valid Earth locations. Their units remain true metres
inherited from NZTM2000.

Earth coordinates remain provenance and processing space only. Mudlib-facing
records expose fictional grid addresses, never Earth longitude or latitude.

## Retained physical extent

The retained source contains the two major islands plus every smaller island
wholly inside the selected square. Islands outside or crossing that boundary
are omitted whole rather than clipped. The retained coastline contains six
features comprising 24 polygon parts and 265,541.831 square kilometres of land.
No proper geographic names are introduced.

`scope/world_scope.json` and `scope/world_boundary.gpkg` are the authoritative
working scope records. The physical edition is not yet frozen.

## Binary world frame

The complete fictional address space is a square with these exact dimensions:

| Level | Per edge | Child structure |
|---|---:|---:|
| whole world | 1,310,720 m | 80 by 80 regions |
| atlas region | 16,384 m | 64 by 64 wilderness squares |
| wilderness square | 256 m | final occupiable spatial unit |

The world therefore has 5,120 square addresses per edge and 26,214,400
possible addresses before ocean is omitted. Its closest land approach has
approximately 28.781 km of ocean clearance. Ocean remains fully represented in
canonical QGIS evidence but ocean squares and ocean-only regions are not
serialized to the mudlib.

The southwest corner of `wb.0` is the address origin. World cell and region
indices are zero-based and increase east and north:

```text
cell_x   = floor((x - world_min_x) / 256)
cell_y   = floor((y - world_min_y) / 256)
region_x = floor(cell_x / 64)
region_y = floor(cell_y / 64)
local_x  = cell_x mod 64
local_y  = cell_y mod 64
```

The addressable frame is half-open on its maximum edges: minimum coordinates
are included and coordinates exactly equal to `world_max_x` or `world_max_y`
are outside. A cell center is at the southwest corner plus `(cell_x + 0.5,
cell_y + 0.5) * 256 m`. No screen-origin or north-down raster row convention
changes these public world-address semantics.

The 256 m square is a downstream aggregation footprint. Finer QGIS elevation,
hydrology, landform, climate, soil, and ecological samples are evidence—not
rooms, places, atlas squares, identifiers, or mudlib coordinates.

## Metric candidate evidence

The current corrected working artifacts are:

- `metric_candidate/unnamed_world_metric_candidate.qgz`;
- `metric_candidate/world_physical_metric_candidate.gpkg`;
- `metric_candidate/glo30_250m_world_metric_candidate.tif`;
- `metric_candidate/unnamed_world_metric_candidate_preview.png`;
- `metric_candidate/metric_world_candidate_qa.json`;
- `world_transform_metric.json`.

The `250m` raster filename describes a provisional internal evidence tier. It
does not define an occupiable square. The corrected retained land area is
equivalent to approximately 4,051,846.780 complete 256 m square areas; this is
an area ratio, not the eventual exact occupiable-square count because coastal
squares are selected by the final land-and-inland-water rule.

## Vertical independence

Horizontal rotation does not change elevation. Canonical heights use NZVD2016
under `terrain/VERTICAL_REFERENCE_POLICY.md`. A horizontally correct candidate
is not hydrology-admitted until source-specific vertical lineage, uncertainty,
bare-earth status, seams, voids, and conditioning pass their independent gates.

All terrain, hydrology, climate, geology, ecology, and downstream export work
must use this contract. It becomes immutable only when the complete physical
world edition is frozen.

`coordinate_contract_qa.json` is the executable audit result produced by
`validate_coordinate_contract.py`. It binds this contract to the saved QGIS
project, transform record, retained scope, and binary region grid.
