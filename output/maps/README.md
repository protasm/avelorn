# Avelorn reference maps

## Adopted working borders: version 1

[Full North Island](north-island-pre-conquest-cell-borders-draft-01.png) ·
[River corridor CC–GG, rows 15–18](north-island-cell-borders-river-corridor-draft-01.png) ·
[Northern border including HH12](north-island-cell-borders-northern-border-draft-01.png)

Refines the Draft 05 pink corridors into individual 256 m square assignments.
The entire island has one country per square; solid region assignments are
retained. Border lines lie on exact shared square edges. The river enlargement
shows how the routes use channels and high-catchment flow paths through
CC17–FF17. Fine grid lines in the enlargements show individual atlas cells.

The authoritative descriptor is `atlas/political/pre-conquest/working-borders.json`.
The 1,740,269-square political overlay and validation report are saved under
`atlas/political/pre-conquest/versions/001/`; format and rebuilding instructions
are in `atlas/political/README.md`. There are 492 uniform and 50 divided regions.
All five countries are connected, and the central country remains landlocked
with the full Great Lake and volcanic summit inside it. These maps were adopted
as working version 1 on September 6, 2026; their original draft labels document
their review provenance. The game does not yet load these political records.

Blue now includes flow paths with upstream catchment of at least 10,000 hectares,
as well as classified channels and inland water. This exposes hydrological
continuity through squares whose dominant terrain is wetland, forest, or other
land cover. No physical atlas data was edited.

## North Island — Edition 1 Region Grid

[Open the full-resolution map](north-island-region-grid.png)

Created September 6, 2026, as a reference for planning the five nations' historic
territories before the Conquest. No national borders have been assigned.

- Source: frozen Edition 1 physical atlas JSON in
  `atlas/qgis/unnamed_new_zealand/export/edition1_256m_atlas/`.
- Extent: the connected North Island landmass containing square `(113, 3336)`;
  1,740,269 atlas squares spanning 542 populated regions. Separate offshore
  islands are not included.
- Grid: existing atlas regions, each 64 × 64 squares of 256 metres, or
  16.384 kilometres per side.
- Addresses: `(column, row)`, increasing east and north. The displayed grid
  spans columns 1–52 and rows 48–76 and extends across ocean for reference.
- Appearance: elevation coloring and relief shading derived from JSON heights;
  inland water indicated from terrain codes. The Great Lake and volcanic
  mountain are labeled.
- Image: `north-island-region-grid.png`, 3532 × 2171 pixels.

This image is a planning reference derived from the atlas, not a replacement
for its source JSON or a definition of political territory.

## Five Nations Before the Conquest — Ownership Draft 01

[Open the ownership sketch](north-island-pre-conquest-ownership-draft-01.png)

Provisional regional coloring of the same North Island extent. Blue, gold,
teal, and purple identify western, northern, eastern, and southern coastal
territories; green identifies the central highlands. Pink marks regions whose
North Island squares fall into more than one proposed territory and therefore
need boundary refinement. Full region boxes are tinted, including their ocean
portions; these are not claims to maritime territory.

The central proposal encloses the Great Lake and volcanic summit and was
checked to have no ocean-adjacent squares. All 542 North Island regions are
represented. The division is a geometric first sketch, not a surveyed boundary
following individual rivers and ridgelines.

The companion `north-island-pre-conquest-ownership-draft-01.json` records the
draft's region colors, candidate nations, coastal seed positions, and central
polygon for later revision. It is planning data only and is not loaded by the
game. Nation names and final borders remain undecided.

## Five Nations Before the Conquest — Ownership Draft 02

[Open the revised ownership sketch](north-island-pre-conquest-ownership-draft-02.png)

Revises Draft 01 with two requested river-oriented boundary shifts. In the
discussion's **(row, column)** notation: `(61,30)` moves to `(60,30)` and
`(61,31)` moves to `(61,32)`. The two vacated regions become northern coastal
territory; the two destination regions become pink boundary regions. All other
region colors remain as in Draft 01.

The companion `north-island-pre-conquest-ownership-draft-02.json` records these
explicit region overrides using the atlas's **(column, row)** convention.
These overrides supersede the initial geometric sketch; the exact river course
within each boundary region has not yet been delineated. Draft 01 is retained.

Draft 02 displays columns as A–Z followed by AA, BB, CC, through ZZ, as requested
(repeated letters, not spreadsheet numbering). A maps to atlas column 1, Z to
26, AA to 27, and ZZ to 52. Rows retain their atlas numbers. The revised pink
regions are DD60 and FF61; DD61 and EE61 become northern coastal territory.
Underlying numeric atlas coordinates are unchanged. The JSON includes the
display-label mapping.

## Five Nations Before the Conquest — Ownership Draft 03

[Open the corrected ownership sketch](north-island-pre-conquest-ownership-draft-03.png)

Adds the missing north–central boundary regions GG64, HH64, and II64 in pink,
joining the existing pink regions at FF64 and JJ64. These are atlas columns
33–35, row 64. All other region colors and letter labels remain as in Draft 02.
The companion `north-island-pre-conquest-ownership-draft-03.json` records the
updated planning assignments. Earlier drafts are retained.

## Five Nations Before the Conquest — Ownership Draft 04

[Open the revised ownership sketch](north-island-pre-conquest-ownership-draft-04.png)

Marks HH65 (atlas column 34, row 65) as a pink north–central boundary region.
All other region assignments remain as in Draft 03. The companion
`north-island-pre-conquest-ownership-draft-04.json` records the updated planning
assignments. Earlier drafts are retained.

## Five Nations Before the Conquest — Ownership Draft 05

[Open the renumbered ownership sketch](north-island-pre-conquest-ownership-draft-05.png)

Retains all Draft 04 territory assignments and lettered columns. Displayed rows
now run from 1 at the top to 29 at the bottom, on both sides of the map.
Display row = 77 minus atlas region Y. Thus the previous HH65 boundary region
is now labeled HH12. Underlying atlas coordinates retain their original axes.
The companion `north-island-pre-conquest-ownership-draft-05.json` includes both
column and row display mappings. Earlier drafts are retained.
