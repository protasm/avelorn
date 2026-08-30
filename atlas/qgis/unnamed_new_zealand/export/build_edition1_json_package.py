#!/usr/bin/env python3
"""Build the sparse, gzip-sharded mudlib JSON package from frozen Edition 1."""

from __future__ import annotations

import gzip
import hashlib
import json
import shutil
from datetime import datetime, timezone
from pathlib import Path

import numpy as np
from osgeo import gdal


ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "export/edition1_256m_atlas"
REGIONS = OUTPUT / "regions"
TEMP_REGIONS = OUTPUT / "regions.tmp"
LAND = ROOT / "terrain/analysis_256m_edition1/land_mask.tif"
ELEVATION = ROOT / "terrain/analysis_256m_edition1/elevation_m.tif"
TERRAIN = OUTPUT / "rasters/terrain_code.tif"
TERRAIN_TYPES = OUTPUT / "terrain_types.json"
TERRAIN_VALIDATION = OUTPUT / "terrain_classification_validation.json"
FREEZE_MANIFEST = ROOT / "EDITION1_FREEZE_MANIFEST.json"
FROZEN_PROJECT = ROOT / "metric_candidate/avelorn_physical_edition1.qgz"
CONTRACT = ROOT / "export/ATLAS_JSON_CONTRACT.md"
WORLD = OUTPUT / "world.json"
PACKAGE = OUTPUT / "package.json"
REGION_SCHEMA = OUTPUT / "region.schema.json"
PACKAGE_SCHEMA = OUTPUT / "package.schema.json"
BUILD_QA = OUTPUT / "build_qa.json"

REGIONS_PER_AXIS = 80
CELLS_PER_REGION_AXIS = 64
CELLS_PER_WORLD_AXIS = 5120
CELL_EDGE_METRES = 256
REGION_EDGE_METRES = 16384


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def compact_json_bytes(value: object) -> bytes:
    return (json.dumps(value, ensure_ascii=True, separators=(",", ":"), sort_keys=True) + "\n").encode("utf-8")


def write_json(path: Path, value: object) -> None:
    path.write_bytes(compact_json_bytes(value))


def write_gzip_json(path: Path, value: object) -> int:
    payload = compact_json_bytes(value)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("wb") as raw:
        with gzip.GzipFile(filename="", mode="wb", fileobj=raw, mtime=0, compresslevel=9) as stream:
            stream.write(payload)
    return len(payload)


def read_exact(path: Path, template: gdal.Dataset) -> np.ndarray:
    source = gdal.Open(str(path), gdal.GA_ReadOnly)
    if source is None:
        raise FileNotFoundError(path)
    if (
        source.RasterXSize != template.RasterXSize
        or source.RasterYSize != template.RasterYSize
        or source.GetGeoTransform() != template.GetGeoTransform()
        or source.GetProjection() != template.GetProjection()
    ):
        raise RuntimeError(f"JSON package input is not on the exact 256 m grid: {path}")
    return source.GetRasterBand(1).ReadAsArray()


def main() -> None:
    gdal.UseExceptions()
    OUTPUT.mkdir(parents=True, exist_ok=True)
    terrain_validation = json.loads(TERRAIN_VALIDATION.read_text())
    freeze = json.loads(FREEZE_MANIFEST.read_text())
    if terrain_validation.get("status") != "pass":
        raise RuntimeError("terrain classifier has not passed independent validation")
    if sha256(FROZEN_PROJECT) != freeze.get("qgis_project_sha256"):
        raise RuntimeError("frozen QGIS project does not match its freeze manifest")
    template = gdal.Open(str(LAND), gdal.GA_ReadOnly)
    if template is None:
        raise FileNotFoundError(LAND)
    if (template.RasterXSize, template.RasterYSize) != (CELLS_PER_WORLD_AXIS, CELLS_PER_WORLD_AXIS):
        raise RuntimeError("Edition 1 analysis grid is not 5120 by 5120")
    land = template.GetRasterBand(1).ReadAsArray() == 1
    elevation = read_exact(ELEVATION, template)
    terrain = read_exact(TERRAIN, template).astype(np.uint8, copy=False)
    if not np.array_equal(terrain > 0, land):
        raise RuntimeError("terrain raster and hard coast disagree")

    if TEMP_REGIONS.exists():
        shutil.rmtree(TEMP_REGIONS)
    TEMP_REGIONS.mkdir(parents=True)
    region_entries = []
    total_squares = 0
    total_plain_bytes = 0
    maximum_region_squares = 0
    for region_y in range(REGIONS_PER_AXIS):
        raster_row_start = CELLS_PER_WORLD_AXIS - (region_y + 1) * CELLS_PER_REGION_AXIS
        raster_row_end = raster_row_start + CELLS_PER_REGION_AXIS
        for region_x in range(REGIONS_PER_AXIS):
            column_start = region_x * CELLS_PER_REGION_AXIS
            column_end = column_start + CELLS_PER_REGION_AXIS
            land_block = land[raster_row_start:raster_row_end, column_start:column_end][::-1, :]
            local_indices = np.flatnonzero(land_block.ravel(order="C"))
            if local_indices.size == 0:
                continue
            elevation_block = elevation[raster_row_start:raster_row_end, column_start:column_end][::-1, :].ravel(order="C")
            terrain_block = terrain[raster_row_start:raster_row_end, column_start:column_end][::-1, :].ravel(order="C")
            squares = [
                [int(local_index), int(elevation_block[local_index]), int(terrain_block[local_index])]
                for local_index in local_indices
            ]
            region_index = region_y * REGIONS_PER_AXIS + region_x
            relative = Path("regions") / f"x{region_x:02d}" / f"r{region_x:02d}_{region_y:02d}.json.gz"
            temporary = TEMP_REGIONS / f"x{region_x:02d}" / f"r{region_x:02d}_{region_y:02d}.json.gz"
            document = {
                "schema": 1,
                "region_index": region_index,
                "region_x": region_x,
                "region_y": region_y,
                "squares": squares,
            }
            plain_bytes = write_gzip_json(temporary, document)
            total_plain_bytes += plain_bytes
            total_squares += len(squares)
            maximum_region_squares = max(maximum_region_squares, len(squares))
            region_entries.append({
                "region_index": region_index,
                "region_x": region_x,
                "region_y": region_y,
                "path": str(relative),
                "square_count": len(squares),
                "sha256": sha256(temporary),
            })
    if total_squares != int(np.count_nonzero(land)):
        raise RuntimeError(f"JSON region total {total_squares} does not equal land total")
    if REGIONS.exists():
        shutil.rmtree(REGIONS)
    TEMP_REGIONS.replace(REGIONS)

    world = {
        "schema": 1,
        "edition": "edition1",
        "coordinate_semantics": {
            "origin": "southwest corner of frozen world square",
            "positive_x": "fictional east",
            "positive_y": "fictional north",
            "qgis_screen_up_at_rotation_zero": "fictional north",
        },
        "world_side_metres": CELLS_PER_WORLD_AXIS * CELL_EDGE_METRES,
        "regions_per_axis": REGIONS_PER_AXIS,
        "region_edge_metres": REGION_EDGE_METRES,
        "cells_per_region_axis": CELLS_PER_REGION_AXIS,
        "cell_edge_metres": CELL_EDGE_METRES,
        "local_index_formula": "local_y * 64 + local_x",
        "ocean_squares_stored": False,
        "square_physical_fields": ["elevation_m", "terrain_code"],
    }
    write_json(WORLD, world)
    package = {
        "schema": 1,
        "edition": "edition1",
        "state": "derived from frozen canonical QGIS Edition 1",
        "frozen_qgis_project": str(FROZEN_PROJECT.relative_to(ROOT)),
        "frozen_qgis_project_sha256": sha256(FROZEN_PROJECT),
        "freeze_manifest": str(FREEZE_MANIFEST.relative_to(ROOT)),
        "freeze_manifest_sha256": sha256(FREEZE_MANIFEST),
        "world": "world.json",
        "terrain_types": "terrain_types.json",
        "region_schema": "region.schema.json",
        "regions": region_entries,
    }
    write_json(PACKAGE, package)
    region_schema = {
        "$schema": "https://json-schema.org/draft/2020-12/schema",
        "type": "object",
        "required": ["schema", "region_index", "region_x", "region_y", "squares"],
        "properties": {
            "schema": {"const": 1},
            "region_index": {"type": "integer", "minimum": 0, "maximum": 6399},
            "region_x": {"type": "integer", "minimum": 0, "maximum": 79},
            "region_y": {"type": "integer", "minimum": 0, "maximum": 79},
            "squares": {
                "type": "array",
                "items": {
                    "type": "array", "minItems": 3, "maxItems": 3,
                    "prefixItems": [
                        {"type": "integer", "minimum": 0, "maximum": 4095},
                        {"type": "integer"},
                        {"type": "integer", "minimum": 1, "maximum": 62},
                    ],
                },
            },
        },
        "additionalProperties": False,
    }
    package_schema = {
        "$schema": "https://json-schema.org/draft/2020-12/schema",
        "type": "object",
        "required": ["schema", "edition", "state", "frozen_qgis_project", "frozen_qgis_project_sha256", "freeze_manifest", "freeze_manifest_sha256", "world", "terrain_types", "region_schema", "regions"],
        "properties": {
            "schema": {"const": 1},
            "edition": {"const": "edition1"},
            "state": {"type": "string"},
            "frozen_qgis_project": {"type": "string"},
            "frozen_qgis_project_sha256": {"type": "string"},
            "freeze_manifest": {"type": "string"},
            "freeze_manifest_sha256": {"type": "string"},
            "world": {"const": "world.json"},
            "terrain_types": {"const": "terrain_types.json"},
            "region_schema": {"const": "region.schema.json"},
            "regions": {"type": "array", "items": {"type": "object"}},
        },
        "additionalProperties": False,
    }
    write_json(REGION_SCHEMA, region_schema)
    write_json(PACKAGE_SCHEMA, package_schema)

    gzip_bytes = sum((OUTPUT / entry["path"]).stat().st_size for entry in region_entries)
    qa = {
        "schema": 1,
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "status": "pass",
        "state": "complete downstream Edition 1 land-only JSON package; pending independent round-trip validation",
        "land_square_count": total_squares,
        "region_file_count": len(region_entries),
        "ocean_square_count_stored": 0,
        "maximum_region_square_count": maximum_region_squares,
        "uncompressed_region_json_bytes": total_plain_bytes,
        "compressed_region_json_bytes": gzip_bytes,
        "compression_ratio": gzip_bytes / total_plain_bytes,
        "large_array_paths": {"package": "/regions", "region_file": "/squares"},
        "square_entry_shape": ["local_index", "elevation_m", "terrain_code"],
        "exact_64_bit_integers_required": False,
        "null_false_distinction_required": False,
        "ocean_regions_or_squares_stored": False,
        "inputs": {
            str(path.relative_to(ROOT)): sha256(path)
            for path in [FROZEN_PROJECT, FREEZE_MANIFEST, LAND, ELEVATION, TERRAIN, TERRAIN_TYPES, TERRAIN_VALIDATION, CONTRACT]
        },
        "outputs": {
            str(path.relative_to(ROOT)): sha256(path)
            for path in [WORLD, PACKAGE, TERRAIN_TYPES, REGION_SCHEMA, PACKAGE_SCHEMA]
        },
        "proper_geographic_names_introduced": False,
    }
    BUILD_QA.write_text(json.dumps(qa, indent=2, sort_keys=True) + "\n")
    print(json.dumps({
        "status": qa["status"],
        "land_squares": total_squares,
        "region_files": len(region_entries),
        "compressed_region_bytes": gzip_bytes,
        "compression_ratio": qa["compression_ratio"],
        "ocean_squares_stored": 0,
    }, sort_keys=True))


if __name__ == "__main__":
    main()
