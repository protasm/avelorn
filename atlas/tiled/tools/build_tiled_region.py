#!/usr/bin/env python3
"""Build a Tiled world and region map from the frozen Edition 1 JSON atlas."""

from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import os
import re
import struct
import zlib
from pathlib import Path


TILED_VERSION = "1.12.2"
FORMAT_VERSION = "1.10"
REGIONS_PER_AXIS = 80
CELLS_PER_REGION_AXIS = 64
TILE_SIZE = 16
TILESET_COLUMNS = 8
TERRAIN_CODE_COUNT = 64

TILED_ROOT = Path(__file__).resolve().parents[1]
REPO_ROOT = Path(__file__).resolve().parents[3]
ATLAS_ROOT = (
    REPO_ROOT
    / "atlas/qgis/unnamed_new_zealand/export/edition1_256m_atlas"
)
TERRAIN_TYPES = ATLAS_ROOT / "terrain_types.json"
MAPS_ROOT = TILED_ROOT / "maps"
TILESETS_ROOT = TILED_ROOT / "tilesets"
PROJECT = TILED_ROOT / "avelorn.tiled-project"
WORLD = TILED_ROOT / "avelorn.world"
TILESET = TILESETS_ROOT / "edition1-terrain.tsj"
TILESET_IMAGE = TILESETS_ROOT / "edition1-terrain.png"


def compact_json(value: object) -> str:
    return json.dumps(value, ensure_ascii=True, indent=2, sort_keys=False) + "\n"


def write_json(path: Path, value: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(compact_json(value), encoding="utf-8")


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def png_chunk(kind: bytes, payload: bytes) -> bytes:
    return (
        struct.pack(">I", len(payload))
        + kind
        + payload
        + struct.pack(">I", zlib.crc32(kind + payload) & 0xFFFFFFFF)
    )


def terrain_base_colour(terrain_id: str) -> tuple[int, int, int]:
    if terrain_id == "non-occupiable-ocean-or-outside":
        return (0, 0, 0)
    if "snow" in terrain_id or "ice" in terrain_id:
        return (226, 238, 241)
    if any(word in terrain_id for word in ("water", "river", "lake")):
        return (48, 116, 153)
    if any(word in terrain_id for word in ("wetland", "estuarine", "peat")):
        return (52, 128, 121)
    if any(word in terrain_id for word in ("coast", "dune")):
        return (196, 176, 116)
    if "volcanic" in terrain_id or "ultramafic" in terrain_id:
        return (126, 87, 70)
    if any(
        word in terrain_id
        for word in (
            "rock", "ridge", "cliff", "scree", "talus", "blockfield",
            "crystalline", "metamorphic",
        )
    ):
        return (123, 126, 122)
    if "forest" in terrain_id:
        return (47, 103, 61)
    if "woodland" in terrain_id:
        return (83, 124, 68)
    if "scrub" in terrain_id or "shrubland" in terrain_id:
        return (112, 128, 70)
    if any(word in terrain_id for word in ("grass", "plain", "valley")):
        return (139, 153, 79)
    if any(word in terrain_id for word in ("alpine", "highland", "plateau")):
        return (142, 137, 116)
    return (112, 137, 82)


def adjusted_colour(base: tuple[int, int, int], code: int) -> tuple[int, int, int]:
    adjustment = ((code * 17) % 23) - 11
    return tuple(max(18, min(242, component + adjustment)) for component in base)


def build_tileset_png(terrain_types: list[dict[str, object]]) -> bytes:
    width = TILESET_COLUMNS * TILE_SIZE
    height = (TERRAIN_CODE_COUNT // TILESET_COLUMNS) * TILE_SIZE
    pixels = bytearray(width * height * 4)
    terrain_by_code = {int(item["code"]): item for item in terrain_types}

    for code in range(TERRAIN_CODE_COUNT):
        terrain = terrain_by_code[code]
        terrain_id = str(terrain["id"])
        tile_column = code % TILESET_COLUMNS
        tile_row = code // TILESET_COLUMNS
        base = adjusted_colour(terrain_base_colour(terrain_id), code)
        for local_y in range(TILE_SIZE):
            for local_x in range(TILE_SIZE):
                image_x = tile_column * TILE_SIZE + local_x
                image_y = tile_row * TILE_SIZE + local_y
                offset = (image_y * width + image_x) * 4
                if code == 0:
                    rgba = (0, 0, 0, 0)
                elif local_x in (0, TILE_SIZE - 1) or local_y in (0, TILE_SIZE - 1):
                    rgba = tuple(max(0, value - 28) for value in base) + (255,)
                elif (local_x + local_y + code) % 11 == 0:
                    rgba = tuple(min(255, value + 22) for value in base) + (255,)
                else:
                    rgba = base + (255,)
                pixels[offset:offset + 4] = bytes(rgba)

    scanlines = b"".join(
        b"\x00" + pixels[row * width * 4:(row + 1) * width * 4]
        for row in range(height)
    )
    return (
        b"\x89PNG\r\n\x1a\n"
        + png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0))
        + png_chunk(b"IDAT", zlib.compress(scanlines, level=9))
        + png_chunk(b"IEND", b"")
    )


def build_tileset(terrain_types: list[dict[str, object]]) -> dict[str, object]:
    tiles = []
    for terrain in terrain_types:
        tiles.append({
            "id": int(terrain["code"]),
            "class": "AvelornTerrain",
            "properties": [
                {"name": "terrain_code", "type": "int", "value": int(terrain["code"])},
                {"name": "terrain_id", "type": "string", "value": str(terrain["id"])},
                {"name": "edition1_state", "type": "string", "value": str(terrain["state"])},
                {
                    "name": "edition1_land_cell_count",
                    "type": "int",
                    "value": int(terrain["land_cell_count"]),
                },
            ],
        })
    return {
        "columns": TILESET_COLUMNS,
        "image": TILESET_IMAGE.name,
        "imageheight": (TERRAIN_CODE_COUNT // TILESET_COLUMNS) * TILE_SIZE,
        "imagewidth": TILESET_COLUMNS * TILE_SIZE,
        "margin": 0,
        "name": "Edition 1 Terrain",
        "spacing": 0,
        "tilecount": TERRAIN_CODE_COUNT,
        "tiledversion": TILED_VERSION,
        "tileheight": TILE_SIZE,
        "tiles": tiles,
        "tilewidth": TILE_SIZE,
        "type": "tileset",
        "version": FORMAT_VERSION,
    }


def region_source(region_x: int, region_y: int) -> Path:
    return (
        ATLAS_ROOT
        / "regions"
        / f"x{region_x:02d}"
        / f"r{region_x:02d}_{region_y:02d}.json.gz"
    )


def read_region(path: Path) -> dict[str, object]:
    with gzip.open(path, "rt", encoding="utf-8") as stream:
        return json.load(stream)


def build_region_map(
    document: dict[str, object], source: Path, source_digest: str
) -> dict[str, object]:
    region_x = int(document["region_x"])
    region_y = int(document["region_y"])
    data = [0] * (CELLS_PER_REGION_AXIS * CELLS_PER_REGION_AXIS)
    seen: set[int] = set()

    for entry in document["squares"]:
        local_index, _elevation_m, terrain_code, _catchment_ha = map(int, entry)
        if local_index in seen:
            raise ValueError(f"duplicate local_index {local_index}")
        if not 0 <= local_index < len(data):
            raise ValueError(f"local_index out of range: {local_index}")
        if not 1 <= terrain_code <= 62:
            raise ValueError(f"land terrain code out of range: {terrain_code}")
        seen.add(local_index)
        local_y, local_x = divmod(local_index, CELLS_PER_REGION_AXIS)
        tiled_row = CELLS_PER_REGION_AXIS - 1 - local_y
        data[tiled_row * CELLS_PER_REGION_AXIS + local_x] = terrain_code + 1

    source_from_map = os.path.relpath(source, MAPS_ROOT)
    return {
        "compressionlevel": -1,
        "height": CELLS_PER_REGION_AXIS,
        "infinite": False,
        "layers": [
            {
                "data": data,
                "height": CELLS_PER_REGION_AXIS,
                "id": 1,
                "locked": True,
                "name": "Physical Terrain (Edition 1)",
                "opacity": 1,
                "type": "tilelayer",
                "visible": True,
                "width": CELLS_PER_REGION_AXIS,
                "x": 0,
                "y": 0,
            },
            {
                "draworder": "topdown",
                "id": 2,
                "name": "Areas",
                "objects": [],
                "opacity": 1,
                "type": "objectgroup",
                "visible": True,
                "x": 0,
                "y": 0,
            },
            {
                "draworder": "topdown",
                "id": 3,
                "name": "Entrances",
                "objects": [],
                "opacity": 1,
                "type": "objectgroup",
                "visible": True,
                "x": 0,
                "y": 0,
            },
        ],
        "nextlayerid": 4,
        "nextobjectid": 1,
        "orientation": "orthogonal",
        "properties": [
            {"name": "atlas_edition", "type": "string", "value": "edition1"},
            {"name": "atlas_region_x", "type": "int", "value": region_x},
            {"name": "atlas_region_y", "type": "int", "value": region_y},
            {
                "name": "atlas_world_x_min",
                "type": "int",
                "value": region_x * CELLS_PER_REGION_AXIS,
            },
            {
                "name": "atlas_world_y_min",
                "type": "int",
                "value": region_y * CELLS_PER_REGION_AXIS,
            },
            {"name": "source_region", "type": "file", "value": source_from_map},
            {"name": "source_sha256", "type": "string", "value": source_digest},
            {
                "name": "source_square_count",
                "type": "int",
                "value": len(document["squares"]),
            },
        ],
        "renderorder": "right-down",
        "tiledversion": TILED_VERSION,
        "tileheight": TILE_SIZE,
        "tilesets": [{"firstgid": 1, "source": "../tilesets/edition1-terrain.tsj"}],
        "tilewidth": TILE_SIZE,
        "type": "map",
        "version": FORMAT_VERSION,
        "width": CELLS_PER_REGION_AXIS,
    }


MAP_NAME = re.compile(r"^r(?P<x>\d{2})_(?P<y>\d{2})\.tmj$")


def build_world() -> dict[str, object]:
    maps = []
    map_extent = CELLS_PER_REGION_AXIS * TILE_SIZE
    for path in sorted(MAPS_ROOT.glob("r??_??.tmj")):
        match = MAP_NAME.match(path.name)
        if match is None:
            continue
        region_x = int(match.group("x"))
        region_y = int(match.group("y"))
        maps.append({
            "fileName": str(path.relative_to(TILED_ROOT)),
            "height": map_extent,
            "width": map_extent,
            "x": region_x * map_extent,
            "y": (REGIONS_PER_AXIS - 1 - region_y) * map_extent,
        })
    return {"maps": maps, "onlyShowAdjacentMaps": False, "type": "world"}


def validate_outputs(region_map: dict[str, object], world: dict[str, object]) -> None:
    terrain_layer = region_map["layers"][0]
    if len(terrain_layer["data"]) != 4096:
        raise AssertionError("Tiled terrain layer is not 64 by 64")
    if not world["maps"]:
        raise AssertionError("Tiled world contains no maps")
    if region_map["tilesets"][0]["source"] != "../tilesets/edition1-terrain.tsj":
        raise AssertionError("region map tileset path is not portable")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("region_x", type=int)
    parser.add_argument("region_y", type=int)
    args = parser.parse_args()
    if not 0 <= args.region_x < REGIONS_PER_AXIS:
        parser.error("region_x must be between 0 and 79")
    if not 0 <= args.region_y < REGIONS_PER_AXIS:
        parser.error("region_y must be between 0 and 79")

    source = region_source(args.region_x, args.region_y)
    if not source.is_file():
        raise FileNotFoundError(f"Edition 1 contains no populated region at {source}")
    terrain_document = json.loads(TERRAIN_TYPES.read_text(encoding="utf-8"))
    terrain_types = terrain_document["terrain_types"]
    if [int(item["code"]) for item in terrain_types] != list(range(64)):
        raise ValueError("terrain_types.json does not define codes 0 through 63 in order")

    MAPS_ROOT.mkdir(parents=True, exist_ok=True)
    TILESETS_ROOT.mkdir(parents=True, exist_ok=True)
    TILESET_IMAGE.write_bytes(build_tileset_png(terrain_types))
    write_json(TILESET, build_tileset(terrain_types))
    write_json(PROJECT, {
        "automappingRulesFile": "",
        "commands": [],
        "extensionsPath": "",
        "folders": ["."],
        "objectTypesFile": "",
        "propertyTypes": [],
    })

    region_document = read_region(source)
    if (
        int(region_document["region_x"]) != args.region_x
        or int(region_document["region_y"]) != args.region_y
    ):
        raise ValueError("region coordinates do not match its package path")
    region_map = build_region_map(region_document, source, sha256(source))
    map_path = MAPS_ROOT / f"r{args.region_x:02d}_{args.region_y:02d}.tmj"
    write_json(map_path, region_map)
    world = build_world()
    write_json(WORLD, world)
    validate_outputs(region_map, world)

    print(f"Wrote Tiled project: {PROJECT}")
    print(f"Wrote Tiled world:   {WORLD}")
    print(f"Wrote region map:    {map_path}")
    print(f"Imported squares:    {len(region_document['squares'])}")


if __name__ == "__main__":
    main()
