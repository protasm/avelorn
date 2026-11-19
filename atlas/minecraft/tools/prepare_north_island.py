#!/usr/bin/env python3
"""Prepare a separate four-block Minecraft derivative of frozen Edition 1.

The source coordinates stay unchanged. Minecraft x increases east and z south.
Each atlas cell occupies exactly 4x4 blocks; only surface height is interpolated.
Synthetic sea/lake depths and vertical exaggeration are export choices, not atlas
edits. Requires numpy and Pillow; does not require or modify Tiled exports.
"""
from pathlib import Path
import gzip
import hashlib
import importlib.util
import json
import struct
import sys
from collections import deque
import numpy as np
from PIL import Image

ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / "atlas/qgis/unnamed_new_zealand/export/edition1_256m_atlas"
OUT = ROOT / "output/minecraft"


def main():
    sys.dont_write_bytecode = True
    OUT.mkdir(parents=True, exist_ok=True)
    spec = importlib.util.spec_from_file_location("atlas_reader", ROOT / "atlas/tiled/tools/build_tiled_region.py")
    reader = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(reader)
    regions = reader.discover_north_island_regions()
    assert len(regions) == 542, "Unexpected island coverage; review source changes"
    # One full 256-block ocean collar beyond the existing region bounding box.
    xmin = min(x for x, y in regions) * 64 - 64
    xmax = (max(x for x, y in regions) + 1) * 64 + 64
    ymin = min(y for x, y in regions) * 64 - 64
    ymax = (max(y for x, y in regions) + 1) * 64 + 64
    w, h = xmax - xmin, ymax - ymin
    elevation = np.zeros((h, w), dtype=np.float32)
    codes = np.zeros((h, w), dtype=np.uint8)
    hashes = {}
    package = json.loads((SOURCE / "package.json").read_text())
    expected = {r["path"]: r["sha256"] for r in package["regions"]}
    for rx, ry in regions:
        relative = f"regions/x{rx:02d}/r{rx:02d}_{ry:02d}.json.gz"
        path = SOURCE / relative
        digest = hashlib.sha256(path.read_bytes()).hexdigest()
        assert digest == expected[relative], f"Source hash mismatch: {relative}"
        hashes[relative] = digest
        with gzip.open(path, "rt") as stream:
            doc = json.load(stream)
        a = np.asarray(doc["squares"])
        idx = a[:, 0].astype(np.int32)
        x = rx * 64 + idx % 64 - xmin
        z = ymax - 1 - (ry * 64 + idx // 64)
        elevation[z, x] = a[:, 1]
        codes[z, x] = a[:, 2]
    assert np.count_nonzero(codes) == 1740951
    # 12 source elevation metres per vertical block; sea surface is y=62.
    surface = np.maximum(63, 62 + elevation / 12).astype(np.float32)
    water = np.full((h, w), 62, dtype=np.float32)
    # Flatten each connected lake to its median mapped elevation. Beds are
    # synthetic: 6 blocks deep for open water, 2 at shallow margins.
    visited = np.zeros((h, w), dtype=bool)
    lake_mask = (codes == 1) | (codes == 2)
    lake_components = []
    for z, x in zip(*np.nonzero(lake_mask)):
        if visited[z, x]:
            continue
        queue = deque([(int(z), int(x))])
        visited[z, x] = True
        cells = []
        while queue:
            zz, xx = queue.popleft()
            cells.append((zz, xx))
            for nz, nx in ((zz-1,xx),(zz+1,xx),(zz,xx-1),(zz,xx+1)):
                if 0 <= nz < h and 0 <= nx < w and lake_mask[nz,nx] and not visited[nz,nx]:
                    visited[nz,nx] = True
                    queue.append((nz,nx))
        zz, xx = np.asarray(cells).T
        level = float(round(62 + float(np.median(elevation[zz,xx])) / 12))
        level = max(63, level)
        water[zz,xx] = level
        surface[zz,xx] = level - np.where(codes[zz,xx] == 1, 6, 2)
        lake_components.append({"cells":len(cells),"water_y":level})
    rivers = codes == 3
    water[rivers] = np.maximum(63, np.rint(62 + elevation[rivers]/12))
    surface[rivers] = water[rivers] - 2
    # A graduated synthetic sea floor: shallow immediately offshore, 22 blocks
    # below sea level beyond eight atlas cells. No claim of bathymetric fidelity.
    distance = np.full((h,w), 9, dtype=np.uint8)
    distance[codes != 0] = 0
    for step in range(1,10):
        adjacent = np.zeros((h,w), dtype=bool)
        adjacent[1:] |= distance[:-1] < step
        adjacent[:-1] |= distance[1:] < step
        adjacent[:,1:] |= distance[:,:-1] < step
        adjacent[:,:-1] |= distance[:,1:] < step
        distance[(distance == 9) & adjacent] = step
    ocean = codes == 0
    surface[ocean] = 62 - np.minimum(22, 2 + distance[ocean].astype(np.float32)*2.5)
    assert surface.max() < 304, "Mountain exceeds chosen build headroom"
    # Preserve the established western-tip atlas spawn.
    sx, sz = (113-xmin)*4+2, (ymax-1-3336)*4+2
    assert codes[sz//4,sx//4] == 57
    with (OUT / "north-island.bin").open("wb") as stream:
        stream.write(struct.pack(">6i",w,h,sx,sz,xmin,ymax))
        stream.write(surface.astype(">f4").tobytes())
        stream.write(water.astype(">f4").tobytes())
        stream.write(codes.tobytes())
    types = json.loads((SOURCE / "terrain_types.json").read_text())["terrain_types"]
    (OUT / "terrain-ids.txt").write_text("\n".join(t["id"] for t in types)+"\n")
    # Overview is generated directly from source cells, north up.
    palette = np.asarray([reader.terrain_base_colour(t["id"]) if t["code"] else (29,65,94) for t in types],dtype=np.uint8)
    rgb = palette[codes].astype(np.float32)
    dz, dx = np.gradient(elevation)
    shade = np.clip(0.9 + (dx-dz)/350,0.55,1.25)
    shade[ocean] = 1
    preview = Image.fromarray(np.clip(rgb*shade[:,:,None],0,255).astype(np.uint8))
    preview.thumbnail((1800,1100))
    preview.save(OUT / "north-island-overview.png")
    manifest = dict(source_edition="Edition 1 schema 2",source_regions=hashes,
        connected_north_island_cells=1740269,retained_cells=int(np.count_nonzero(codes)),
        horizontal_blocks_per_cell=4,source_metres_per_block=64,
        vertical_metres_per_block=12,sea_level=62,
        dimensions_blocks=[w*4,h*4],atlas_bounds_exclusive=[xmin,ymin,xmax,ymax],
        coordinate_formula={"x":"(atlas_x - xmin) * 4 + 2","z":"(ymax - 1 - atlas_y) * 4 + 2"},
        spawn=[sx,sz],maximum_source_elevation_m=float(elevation.max()),
        maximum_surface_y=float(surface.max()),lake_components=lake_components,
        source_terrain_counts={t["id"]:int(np.count_nonzero(codes==t["code"])) for t in types},
        notes=["Includes 682 small offshore-island cells in boundary regions.",
               "Ocean collar 256 blocks; depths synthetic.",
               "Lake water levels use component median; river levels rounded per source cell.",
               "Small terrain detail and Minecraft vegetation are derivative decoration."])
    (OUT / "build-manifest.json").write_text(json.dumps(manifest,indent=2)+"\n")
    peak_z, peak_x = map(int, np.unravel_index(surface.argmax(), surface.shape))
    lake_z, lake_x = np.nonzero((codes == 1) & (np.indices(codes.shape)[1] // 64 == 35)
        & ((ymax - 1 - np.indices(codes.shape)[0]) // 64 == 60))
    middle = len(lake_x) // 2
    points = {"Western tip": (sz//4, sx//4), "Highest mountain": (peak_z, peak_x),
        "Large inland lake": (int(lake_z[middle]), int(lake_x[middle]))}
    landmarks = []
    for name, (z, x) in points.items():
        flight_y = int(max(surface[z,x],water[z,x]))+20
        landmarks.append(dict(name=name,x=x*4+2,y=flight_y,z=z*4+2,
            atlas_x=x+xmin,atlas_y=ymax-1-z,
            command=f"/tp @s {x*4+2} {flight_y} {z*4+2}"))
    (OUT / "landmarks.json").write_text(json.dumps(landmarks,indent=2)+"\n")
    print(json.dumps({k:v for k,v in manifest.items() if k not in ("source_regions","lake_components","source_terrain_counts")},indent=2),flush=True)


if __name__ == "__main__":
    main()
