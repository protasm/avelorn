# Derived World JSON Contract

## Authority boundary

The canonical QGIS project and its frozen physical-world edition are the source
of truth. JSON is a deterministic downstream compilation target. Editing an
export never changes canonical geometry, rasters, provenance, or uncertainty.
Every exported value must be reproducible from a named canonical edition and an
exporter version.

The export has exactly one spatial resolution: **256 m by 256 m**. Finer source
measurements used inside QGIS are not places, rooms, squares, identifiers, or
coordinates in the atlas or mudlib.

The export is intentionally concerned only with the unoccupied physical world.
It contains no proper geographic names, settlements, roads, political borders,
administrative areas, cultures, or lore.

## Spatial hierarchy

The standard occupiable wilderness square is **256 m by 256 m**. Export files
group these squares inside 16,384 m atlas regions. Both grids share one origin and
the fictional-world axes defined in
`../WORLD_COORDINATE_CONTRACT.md`.

| Level | Edge | Child relationship | Purpose |
|---|---:|---:|---|
| atlas region | 16,384 m | 64 x 64 wilderness squares | file and loading boundary |
| wilderness / atlas square | 256 m | no finer subdivision | standard occupiable MUD location |

The complete address space is exactly 80 by 80 regions, or 5,120 by 5,120
wilderness squares. The world square is therefore 1,310,720 m on each edge.

The hierarchy is address space, not additional per-square world data. A square
is occupiable when its center lies on canonical land or inland water. Ocean is
excluded. Only occupiable squares are serialized.

## Stable identifiers

Identifiers are anonymous, machine-stable addresses rather than names:

- canonical edition: `pwe-YYYYMMDD-NNN`;
- atlas region: `ar.<signed-x>.<signed-y>`;
- wilderness square: `ws.<signed-x>.<signed-y>`;
- source/provenance records retain their provider identifiers unchanged.

Grid indices are derived from the edition's declared grid origin. They must not
be regenerated from feature ordering, display order, or QGIS feature IDs.

## Package layout

The preferred deliverable is a versioned directory of JSON files rather than a
single enormous document:

```text
world-manifest.json
dictionaries/physical-types.json
provenance/sources.json
regions/<x>/<y>.json
```

`world-manifest.json` is the only required entry point. It records the edition,
grid, terrain dictionary, region paths, encodings, and SHA-256 checksum of every
shard. A region contains at most 64 by 64, or 4,096, wilderness squares.
Region shards contain sparse row-major offsets plus exactly two values per
occupiable square. An absent region inside the declared 80 × 80 bounds resolves
to `non-occupiable` without a file; coordinates outside those bounds are outside
the world. This distinction permits ocean omission without conflating ocean with
the exterior of the atlas.

The canonical QGIS edition still retains coastline, water, and shallow-seafloor
complexity. Omitting occupiable ocean squares is strictly a downstream export
decision and does not remove canonical physical evidence.

The release does not create finer JSON shards or finer occupiable squares.

Every shard is independently replaceable during development, but a frozen
edition is immutable: changing one byte creates a new manifest checksum and a
new edition. `package.schema.json` validates the entry point;
`region.schema.json` validates terrain shards; `world.schema.json` defines the
equivalent logically expanded world for tools that require a monolith.

## Required semantics

- Distances and elevations are metres; areas are square metres; slopes are
  degrees; aspects are degrees clockwise from fictional north.
- Coordinates use fictional east and fictional north. Earth longitude and
  latitude are prohibited in mudlib-facing spatial records.
- Each occupiable square has exactly two physical values:
  `elevation_m` and `terrain_code`.
- `elevation_m` is the area-weighted mean canonical land elevation within the
  exact 256 m footprint, rounded to the nearest metre. For inland water it is
  the modeled water-surface elevation.
- `terrain_code` is the terrain class covering the greatest land area inside
  the exact footprint. Ties resolve by the dictionary's fixed priority and then
  by the lower numeric code.
- The terrain dictionary contains no more than 64 anonymous physical classes,
  represented by codes 1 through 64. Unused codes may remain explicitly
  reserved in a frozen edition; code assignments never shift within an edition.
  Code zero is reserved and never denotes an occupiable square.
- Visibility, movement cost, adjacency, weather effects, and descriptions are
  runtime derivations and are not stored in the physical square record.
- Fine elevation statistics, hydrology, geology, soils, vegetation evidence,
  uncertainty, and provenance remain in the canonical QGIS edition and build
  reports rather than being duplicated per mudlib square.

## Versioning and determinism

The package validates against its applicable schemas. A canonical export
records the physical-world edition, coordinate-contract checksum, source
manifest checksums, exporter identity, exporter version, export profile, and a
chronological derivation operation list. Object arrays are serialized in stable
identifier order and JSON object keys are lexicographically sorted before the
release checksum is computed. Paths are relative to the manifest, may not escape
the package, and use forward slashes on every platform.

Breaking semantic changes increment `schema_version`. Additive optional fields
increment the exporter version. A frozen physical-world edition is never
silently rebuilt from newer source catalogs.
