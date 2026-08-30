#!/usr/bin/env python3
"""Derive the mudlib terrain code from frozen Edition 1 physical evidence."""

from __future__ import annotations

import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

import numpy as np
from osgeo import gdal
from scipy import ndimage


ROOT = Path(__file__).resolve().parents[1]
FROZEN = ROOT / "metric_candidate/avelorn_physical_edition1.qgz"
FREEZE_MANIFEST = ROOT / "EDITION1_FREEZE_MANIFEST.json"
ANALYSIS = ROOT / "terrain/analysis_256m_edition1"
OUTPUT_DIR = ROOT / "export/edition1_256m_atlas"
RASTER_DIR = OUTPUT_DIR / "rasters"
LAND = ANALYSIS / "land_mask.tif"
ELEVATION = ANALYSIS / "elevation_m.tif"
RIVERS = ANALYSIS / "river_mask.tif"
LAKES = ANALYSIS / "lake_mask.tif"
LANDFORM = ROOT / "terrain/landforms_edition1_v2/landform_class.tif"
SLOPE = ROOT / "terrain/landforms_edition1_v2/slope_degrees.tif"
VEGETATION = ROOT / "terrain/vegetation_edition1_v2/potential_vegetation_structure_256m.tif"
SOIL = ROOT / "terrain/soils_edition1_v2/soil_regime_256m.tif"
COAST_DISTANCE = ROOT / "terrain/vegetation_edition1_v2/distance_inland_from_coast_m_256m.tif"
ACCUMULATION = ROOT / "terrain/hydrology_edition1_v2/flow_accumulation_cells.tif"
TERRAIN = RASTER_DIR / "terrain_code.tif"
DICTIONARY = OUTPUT_DIR / "terrain_types.json"
QA = OUTPUT_DIR / "terrain_classification_qa.json"

TERRAIN_TYPES = [
    "non-occupiable-ocean-or-outside", "open-inland-water", "shallow-lake-margin",
    "river-channel", "river-wetland", "coastal-wetland", "basin-wetland",
    "peat-wetland", "sandy-coast", "rocky-coast", "warm-lowland-closed-forest",
    "temperate-lowland-closed-forest", "wet-lowland-closed-forest",
    "dry-lowland-open-woodland", "lowland-scrub", "lowland-grassland",
    "rolling-humid-forest", "rolling-dry-woodland", "rolling-grassland",
    "forested-valley", "open-valley", "forested-alluvial-plain",
    "open-alluvial-plain", "forested-foothill", "wooded-foothill",
    "scrub-foothill", "grassy-foothill", "cool-montane-forest",
    "wet-montane-forest", "dry-montane-forest", "montane-scrub",
    "montane-grassland", "subalpine-scrub", "subalpine-grassland",
    "alpine-grass-herbfield", "alpine-scree", "bare-alpine-rock",
    "volcanic-plateau", "forested-volcanic-slope", "open-volcanic-slope",
    "forested-rocky-ridge", "open-rocky-ridge", "steep-forested-slope",
    "steep-open-slope", "cliff-or-precipice", "dry-basin-grassland",
    "basin-shrubland", "forested-basin", "high-plateau-scrub",
    "high-plateau-grassland", "persistent-snow-or-ice", "talus-or-blockfield",
    "river-gorge", "lake-basin-margin", "estuarine-wetland", "coastal-forest",
    "coastal-scrub", "dune-grassland", "ultramafic-sparse-ground",
    "crystalline-highland", "metamorphic-highland", "young-alluvial-plain",
    "organic-wetland", "reserved-complex-terrain",
]


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


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
        raise RuntimeError(f"terrain-classifier input is not on the exact 256 m grid: {path}")
    return source.GetRasterBand(1).ReadAsArray()


def write(path: Path, values: np.ndarray, template: gdal.Dataset) -> None:
    output = gdal.GetDriverByName("GTiff").Create(
        str(path), template.RasterXSize, template.RasterYSize, 1, gdal.GDT_Byte,
        options=["TILED=YES", "COMPRESS=DEFLATE", "PREDICTOR=2", "BIGTIFF=IF_SAFER"],
    )
    output.SetGeoTransform(template.GetGeoTransform())
    output.SetProjection(template.GetProjection())
    band = output.GetRasterBand(1)
    band.SetNoDataValue(0)
    band.SetDescription("downstream Edition 1 mudlib terrain code; ocean omitted from JSON")
    band.WriteArray(values)
    band.FlushCache()
    output.FlushCache()


def main() -> None:
    gdal.UseExceptions()
    RASTER_DIR.mkdir(parents=True, exist_ok=True)
    freeze = json.loads(FREEZE_MANIFEST.read_text())
    if sha256(FROZEN) != freeze.get("qgis_project_sha256"):
        raise RuntimeError("frozen QGIS Edition 1 does not match its manifest")
    template = gdal.Open(str(LAND), gdal.GA_ReadOnly)
    if template is None:
        raise FileNotFoundError(LAND)
    land = template.GetRasterBand(1).ReadAsArray() == 1
    elevation = read_exact(ELEVATION, template).astype(np.float32, copy=False)
    rivers = read_exact(RIVERS, template) == 1
    lakes = read_exact(LAKES, template) == 1
    landform = read_exact(LANDFORM, template).astype(np.uint8, copy=False)
    slope = read_exact(SLOPE, template).astype(np.float32, copy=False)
    vegetation = read_exact(VEGETATION, template).astype(np.uint8, copy=False)
    soil = read_exact(SOIL, template).astype(np.uint8, copy=False)
    coast_distance = read_exact(COAST_DISTANCE, template).astype(np.float32, copy=False)
    accumulation = read_exact(ACCUMULATION, template).astype(np.uint32, copy=False)

    terrain = np.zeros(land.shape, dtype=np.uint8)
    terrain[land] = 15
    forest = land & np.isin(vegetation, [1, 2, 3])
    open_land = land & np.isin(vegetation, [4, 5])
    lowland = elevation < 200.0
    foothill = (elevation >= 200.0) & (elevation < 700.0)
    montane = (elevation >= 700.0) & (elevation < 1200.0)
    valley = np.isin(landform, [3, 4])
    ridge = np.isin(landform, [7, 8])
    steep = slope >= 25.0
    very_steep = slope >= 40.0
    wet_soil = np.isin(soil, [2, 4, 12])
    dry_soil = soil == 5

    terrain[forest & lowland & (vegetation == 1)] = 10
    terrain[forest & lowland & (vegetation == 2)] = 11
    terrain[forest & lowland & wet_soil] = 12
    terrain[open_land & lowland & (vegetation == 4)] = 13
    terrain[open_land & lowland & (vegetation == 5)] = 14
    terrain[open_land & lowland & ~dry_soil] = 15
    terrain[forest & np.isin(landform, [1, 2]) & ~lowland] = 16
    terrain[open_land & np.isin(landform, [1, 2]) & dry_soil] = 17
    terrain[open_land & np.isin(landform, [1, 2]) & ~dry_soil] = 18
    terrain[forest & valley] = 19
    terrain[open_land & valley] = 20
    terrain[forest & (soil == 1)] = 21
    terrain[open_land & (soil == 1)] = 22
    terrain[forest & foothill] = 23
    terrain[open_land & foothill & (vegetation == 4)] = 24
    terrain[open_land & foothill & (vegetation == 5) & dry_soil] = 25
    terrain[open_land & foothill & (vegetation == 5) & ~dry_soil] = 26
    terrain[forest & montane] = 27
    terrain[forest & montane & wet_soil] = 28
    terrain[forest & montane & dry_soil] = 29
    terrain[open_land & montane & (vegetation == 5)] = 30
    terrain[open_land & montane & (vegetation == 4)] = 31
    terrain[land & (vegetation == 6)] = 32
    terrain[land & (vegetation == 6) & (landform == 9)] = 33
    terrain[land & (vegetation == 7)] = 34
    terrain[land & (vegetation == 7) & steep] = 35
    terrain[land & (vegetation == 8)] = 36
    terrain[land & (vegetation == 8) & steep] = 51
    terrain[land & (soil == 6) & (landform == 9)] = 37
    terrain[forest & (soil == 6) & ~np.isin(landform, [1, 2, 9])] = 38
    terrain[open_land & (soil == 6) & ~np.isin(landform, [1, 2, 9])] = 39
    terrain[forest & ridge] = 40
    terrain[open_land & ridge] = 41
    terrain[forest & steep] = 42
    terrain[open_land & steep] = 43
    terrain[land & very_steep & (elevation >= 500.0)] = 44
    terrain[open_land & (landform == 4) & dry_soil] = 45
    terrain[open_land & (landform == 4) & ~dry_soil] = 46
    terrain[forest & (landform == 4)] = 47
    terrain[land & (landform == 9) & (vegetation == 6)] = 48
    terrain[land & (landform == 9) & (vegetation == 7)] = 49
    terrain[land & (elevation >= 2500.0)] = 50
    terrain[land & (soil == 9)] = 58
    terrain[land & (soil == 8) & (elevation >= 600.0)] = 59
    terrain[land & (soil == 7) & (elevation >= 600.0)] = 60
    terrain[land & (soil == 1) & np.isin(landform, [1, 2, 3, 4])] = 61
    terrain[land & (soil == 12)] = 62

    wetland = land & (vegetation == 9)
    terrain[wetland] = 6
    terrain[wetland & (soil == 12)] = 62
    terrain[wetland & (coast_distance <= 1000.0)] = 5
    coastal = land & (vegetation == 10)
    terrain[coastal] = 56
    terrain[coastal & (soil == 11) & (slope < 8.0)] = 57
    terrain[coastal & (soil == 11) & (slope >= 8.0)] = 8
    terrain[coastal & (slope >= 15.0)] = 9
    terrain[forest & (coast_distance <= 1000.0)] = 55

    lake_margin = land & ~lakes & ndimage.binary_dilation(lakes, structure=np.ones((3, 3), dtype=np.uint8))
    terrain[lake_margin & valley] = 53
    terrain[rivers] = 3
    terrain[rivers & wetland] = 4
    terrain[rivers & (coast_distance <= 512.0) & (elevation <= 10.0)] = 54
    terrain[rivers & steep & (accumulation >= 1024)] = 52
    lake_interior = lakes & ndimage.binary_erosion(lakes, structure=np.ones((3, 3), dtype=np.uint8), border_value=0)
    terrain[lakes] = 2
    terrain[lake_interior] = 1

    if np.any(terrain[land] == 0) or np.any(terrain[land] == 63) or np.any(terrain[~land] != 0):
        raise RuntimeError("terrain classifier left invalid, reserved, or ocean-coded cells")
    write(TERRAIN, terrain, template)
    counts = np.bincount(terrain.ravel(), minlength=64)
    dictionary = {
        "schema": 1,
        "code_range": [0, 63],
        "terrain_types": [
            {
                "code": code,
                "id": terrain_id,
                "state": "reserved" if code in {0, 63} else "available",
                "land_cell_count": int(np.count_nonzero(terrain[land] == code)),
            }
            for code, terrain_id in enumerate(TERRAIN_TYPES)
        ],
        "proper_geographic_names_introduced": False,
    }
    DICTIONARY.write_text(json.dumps(dictionary, indent=2, sort_keys=True) + "\n")
    qa = {
        "schema": 1,
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "status": "pass",
        "state": "downstream terrain classifier derived from frozen QGIS Edition 1",
        "frozen_qgis_project": str(FROZEN.relative_to(ROOT)),
        "frozen_qgis_project_sha256": sha256(FROZEN),
        "freeze_manifest": str(FREEZE_MANIFEST.relative_to(ROOT)),
        "freeze_manifest_sha256": sha256(FREEZE_MANIFEST),
        "cell_edge_metres": 256,
        "land_cell_count": int(np.count_nonzero(land)),
        "classified_land_cell_count": int(np.count_nonzero(terrain[land] > 0)),
        "ocean_nonzero_cell_count": int(np.count_nonzero(terrain[~land])),
        "reserved_code_63_land_cell_count": int(np.count_nonzero(terrain[land] == 63)),
        "represented_terrain_type_count": int(np.count_nonzero(counts[1:63])),
        "terrain_counts": [
            {"code": code, "id": TERRAIN_TYPES[code], "cell_count": int(counts[code])}
            for code in range(64)
        ],
        "hard_coast_preserved": bool(np.array_equal(terrain > 0, land)),
        "all_lake_cells_are_water_terrain": bool(np.all(np.isin(terrain[lakes], [1, 2]))),
        "all_river_cells_are_contiguous_water_terrain": bool(np.all(np.isin(terrain[rivers], [1, 2, 3, 4, 52, 54]))),
        "inputs": {
            str(path.relative_to(ROOT)): sha256(path)
            for path in [LAND, ELEVATION, RIVERS, LAKES, LANDFORM, SLOPE, VEGETATION, SOIL, COAST_DISTANCE, ACCUMULATION]
        },
        "outputs": {
            str(TERRAIN.relative_to(ROOT)): sha256(TERRAIN),
            str(DICTIONARY.relative_to(ROOT)): sha256(DICTIONARY),
        },
        "proper_geographic_names_introduced": False,
        "provisional_export_used_as_input": False,
    }
    QA.write_text(json.dumps(qa, indent=2, sort_keys=True) + "\n")
    print(json.dumps({
        "status": qa["status"],
        "land_cells": qa["land_cell_count"],
        "represented_terrain_types": qa["represented_terrain_type_count"],
        "hard_coast_preserved": qa["hard_coast_preserved"],
        "lake_water_preserved": qa["all_lake_cells_are_water_terrain"],
        "river_water_preserved": qa["all_river_cells_are_contiguous_water_terrain"],
    }, sort_keys=True))


if __name__ == "__main__":
    main()
