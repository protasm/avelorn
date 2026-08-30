#!/usr/bin/env python3
"""Independently validate the frozen-edition downstream terrain classifier."""

from __future__ import annotations

import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

import numpy as np
from osgeo import gdal
from scipy import ndimage


ROOT = Path(__file__).resolve().parents[1]
PACKAGE = ROOT / "export/edition1_256m_atlas"
LAND = ROOT / "terrain/analysis_256m_edition1/land_mask.tif"
RIVERS = ROOT / "terrain/analysis_256m_edition1/river_mask.tif"
LAKES = ROOT / "terrain/analysis_256m_edition1/lake_mask.tif"
TERRAIN = PACKAGE / "rasters/terrain_code.tif"
DICTIONARY = PACKAGE / "terrain_types.json"
BUILD_QA = PACKAGE / "terrain_classification_qa.json"
FREEZE_MANIFEST = ROOT / "EDITION1_FREEZE_MANIFEST.json"
FROZEN = ROOT / "metric_candidate/avelorn_physical_edition1.qgz"
OUTPUT = PACKAGE / "terrain_classification_validation.json"


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def read(path: Path) -> tuple[gdal.Dataset, np.ndarray]:
    dataset = gdal.Open(str(path), gdal.GA_ReadOnly)
    if dataset is None:
        raise FileNotFoundError(path)
    return dataset, dataset.GetRasterBand(1).ReadAsArray()


def check(name: str, passed: bool, detail: object) -> dict[str, object]:
    return {"name": name, "pass": bool(passed), "detail": detail}


def main() -> None:
    gdal.UseExceptions()
    build = json.loads(BUILD_QA.read_text())
    dictionary = json.loads(DICTIONARY.read_text())
    freeze = json.loads(FREEZE_MANIFEST.read_text())
    land_ds, land_values = read(LAND)
    river_ds, river_values = read(RIVERS)
    lake_ds, lake_values = read(LAKES)
    terrain_ds, terrain = read(TERRAIN)
    datasets = [river_ds, lake_ds, terrain_ds]
    same_grid = all(
        dataset.RasterXSize == land_ds.RasterXSize
        and dataset.RasterYSize == land_ds.RasterYSize
        and dataset.GetGeoTransform() == land_ds.GetGeoTransform()
        and dataset.GetProjection() == land_ds.GetProjection()
        for dataset in datasets
    )
    land = land_values == 1
    rivers = river_values == 1
    lakes = lake_values == 1
    water_codes = {1, 2, 3, 4, 52, 54}
    types = dictionary.get("terrain_types", [])
    codes = [entry.get("code") for entry in types]
    identifiers = [entry.get("id") for entry in types]
    counts = np.bincount(terrain.ravel(), minlength=64)
    land_components, land_component_count = ndimage.label(land, structure=np.ones((3, 3), dtype=np.uint8))
    small_land_components = [
        int(value) for value in np.bincount(land_components.ravel())[1:] if 0 < value <= 4
    ]
    output_details = []
    for relative, expected in build.get("outputs", {}).items():
        path = ROOT / relative
        output_details.append({"path": relative, "matches": path.is_file() and sha256(path) == expected})
    results = [
        check("terrain raster retains the exact 256 m physical grid", same_grid, list(terrain.shape[::-1])),
        check("hard coast is exact and ocean has no terrain entry", np.array_equal(terrain > 0, land), {"missing_land": int(np.count_nonzero(land & (terrain == 0))), "ocean_nonzero": int(np.count_nonzero(~land & (terrain > 0)))}),
        check("all land codes are within 1 through 62", bool(np.all((terrain[land] >= 1) & (terrain[land] <= 62))), sorted(int(value) for value in np.unique(terrain[land]))),
        check("all lake cells retain inland-water terrain", bool(np.all(np.isin(terrain[lakes], [1, 2]))), int(np.count_nonzero(lakes & ~np.isin(terrain, [1, 2])))),
        check("all represented river cells retain contiguous water terrain", bool(np.all(np.isin(terrain[rivers], list(water_codes)))), int(np.count_nonzero(rivers & ~np.isin(terrain, list(water_codes))))),
        check("all 24 island components remain and no lily-pad fragments appear", land_component_count == 24 and not small_land_components, {"components": int(land_component_count), "small_components": small_land_components}),
        check("terrain dictionary defines exactly codes 0 through 63", codes == list(range(64)) and len(set(identifiers)) == 64, {"entry_count": len(types), "codes": codes}),
        check("codes 0 and 63 are reserved and absent from land", types[0].get("state") == "reserved" and types[63].get("state") == "reserved" and counts[63] == 0, {"code_0": types[0].get("state"), "code_63": types[63].get("state"), "code_63_land": int(counts[63])}),
        check("terrain dictionary introduces no proper geographic names", dictionary.get("proper_geographic_names_introduced") is False, dictionary.get("proper_geographic_names_introduced")),
        check("build outputs match declared digests", bool(output_details) and all(item["matches"] for item in output_details), output_details),
        check("classifier is bound to the frozen QGIS project", sha256(FROZEN) == freeze.get("qgis_project_sha256") == build.get("frozen_qgis_project_sha256"), freeze.get("qgis_project_sha256")),
        check("old provisional export was not used as input", build.get("provisional_export_used_as_input") is False, build.get("provisional_export_used_as_input")),
        check("at least forty distinct physical terrains are represented", int(np.count_nonzero(counts[1:63])) >= 40, int(np.count_nonzero(counts[1:63]))),
    ]
    passed = sum(item["pass"] for item in results)
    report = {
        "schema": 1,
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "status": "pass" if passed == len(results) else "fail",
        "checks_passed": passed,
        "checks_total": len(results),
        "checks": results,
        "land_cell_count": int(np.count_nonzero(land)),
        "represented_terrain_type_count": int(np.count_nonzero(counts[1:63])),
    }
    OUTPUT.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    print(json.dumps({"status": report["status"], "checks_passed": passed, "checks_total": len(results), "represented_terrain_types": report["represented_terrain_type_count"]}, sort_keys=True))
    if report["status"] != "pass":
        raise SystemExit(1)


if __name__ == "__main__":
    main()
