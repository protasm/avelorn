# North Island for Minecraft

This is a separate downstream export of frozen physical Edition 1. The atlas,
JSON region files, Tiled maps, and Avelorn runtime are not changed by this build.

## World design

- Every source square becomes exactly 4 × 4 Minecraft blocks: 64 source metres
  per horizontal block. Increasing atlas X is east; increasing atlas Y is north
  and maps to decreasing Minecraft Z.
- All 542 regions intersecting the connected North Island are retained, including
  682 small offshore-island cells. The main connected island has 1,740,269 cells.
- The rectangular map is 13,824 × 7,936 blocks including a 256-block ocean collar.
- Sea level is 62. Elevation uses 12 source metres per vertical block, preserving
  substantial mountain relief with a highest surface near 283 and normal build
  limits of -64 through 319. This exaggerates relief relative to horizontal scale.
- Dry heights are bilinearly interpolated without changing each cell's terrain
  footprint. The source does not contain block-scale terrain detail.
- Lake components use their median source elevation, with synthetic two- or
  six-block beds. River beds are two blocks below rounded per-cell water levels.
  Seafloor depths are synthetic, increasing offshore to 22 blocks below sea level.
- Terrain types become Minecraft surface materials and forest layers. Trees are
  Minecraft decoration, not individually surveyed or canonical Avelorn objects.
- Creative mode, Peaceful difficulty, commands enabled, western-tip starting
  point. Underground resources are generated. There are no authored towns,
  quests, or custom caves; this is an exploration/building world, not a verified
  survival-progression campaign.
- Outside the exported rectangle, Minecraft may generate unrelated terrain.

## Build

Requires Python with NumPy and Pillow, a Java JDK, and WorldPainter 2.27.1.
WorldPainter was obtained from its official website; archive SHA-256:
`f2af630b5ee0e477b2357e00e5c25824cb32286764377abb49935b9f15557317`.

Run `tools/prepare_north_island.py` using that Python environment. It validates
source region hashes against the Edition 1 package and writes its data and
manifest under `output/minecraft/`. Compile `tools/BuildNorthIsland.java` against
the WorldPainter application's `Contents/Resources/app/lib/*` jars and run it
with the same jars and `output/minecraft` as its argument. A 6 GB heap and three
export threads were selected for this 16 GB Mac. The Java program saves an
editable `.world` before exporting. An optional second argument `export` resumes
from that saved editor world; an existing Minecraft destination is never replaced.

WorldPainter's `JAVA_ANVIL_26_1` export platform supports Minecraft 26.1 or later.
The installed game targeted for live verification is Minecraft Java 26.2.

Run `tools/validate_world.py` after export. It verifies every region's location
table and exact chunk coverage, decodes a representative chunk per region and
the landmark chunks, and records results in `output/minecraft/validation.json`.
Live game verification is separate from those structural checks.

## Generated artifacts

- `output/minecraft/North Island 4x.world`: editable WorldPainter project.
- `output/minecraft/saves/Avelorn - North Island (4x)`: original exported save.
- `output/minecraft/north-island-overview.png`: north-up terrain overview.
- `output/minecraft/build-manifest.json`: source hashes and conversion choices.
- `output/minecraft/landmarks.json`: coordinates and teleport commands.
- `output/minecraft/validation.json`: structural validation results.

Large generated artifacts are locally ignored by Git. Keep the original export
separate from the installed playable copy so exploring/building cannot alter the
reproducible source export.
