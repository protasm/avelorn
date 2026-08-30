#!/usr/bin/env python3
"""Round-trip every sparse Edition 1 JSON region against frozen-derived rasters."""

from __future__ import annotations

import gzip
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

import numpy as np
from osgeo import gdal


ROOT = Path(__file__).resolve().parents[1]
PACKAGE_DIR = ROOT / "export/edition1_256m_atlas"
PACKAGE = PACKAGE_DIR / "package.json"
WORLD = PACKAGE_DIR / "world.json"
TERRAIN_TYPES = PACKAGE_DIR / "terrain_types.json"
BUILD_QA = PACKAGE_DIR / "build_qa.json"
LAND = ROOT / "terrain/analysis_256m_edition1/land_mask.tif"
ELEVATION = ROOT / "terrain/analysis_256m_edition1/elevation_m.tif"
TERRAIN = PACKAGE_DIR / "rasters/terrain_code.tif"
FREEZE_MANIFEST = ROOT / "EDITION1_FREEZE_MANIFEST.json"
FROZEN_PROJECT = ROOT / "metric_candidate/avelorn_physical_edition1.qgz"
OUTPUT = PACKAGE_DIR / "package_validation.json"
WORLD_AXIS = 5120
REGION_AXIS = 80
LOCAL_AXIS = 64


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
        raise RuntimeError(f"round-trip input grid mismatch: {path}")
    return source.GetRasterBand(1).ReadAsArray()


def check(name: str, passed: bool, detail: object) -> dict[str, object]:
    return {"name": name, "pass": bool(passed), "detail": detail}


def main() -> None:
    gdal.UseExceptions()
    package = json.loads(PACKAGE.read_text())
    world = json.loads(WORLD.read_text())
    terrain_types = json.loads(TERRAIN_TYPES.read_text())
    build = json.loads(BUILD_QA.read_text())
    freeze = json.loads(FREEZE_MANIFEST.read_text())
    template = gdal.Open(str(LAND), gdal.GA_ReadOnly)
    if template is None:
        raise FileNotFoundError(LAND)
    land = template.GetRasterBand(1).ReadAsArray() == 1
    elevation = read_exact(ELEVATION, template)
    terrain = read_exact(TERRAIN, template).astype(np.uint8, copy=False)
    seen = np.zeros(land.shape, dtype=bool)
    terrain_counts = np.zeros(64, dtype=np.int64)
    region_details = []
    duplicate_count = 0
    value_mismatch_count = 0
    invalid_entry_count = 0
    total_squares = 0
    region_entries = package.get("regions", [])
    for entry in region_entries:
        path = PACKAGE_DIR / entry["path"]
        gzip_signature = path.read_bytes()[:2] == b"\x1f\x8b" if path.is_file() else False
        digest_matches = path.is_file() and sha256(path) == entry.get("sha256")
        with gzip.open(path, "rt", encoding="utf-8") as stream:
            document = json.load(stream)
        squares = document.get("squares")
        header_matches = (
            document.get("schema") == 1
            and document.get("region_index") == entry.get("region_index")
            and document.get("region_x") == entry.get("region_x")
            and document.get("region_y") == entry.get("region_y")
            and isinstance(squares, list)
            and len(squares) == entry.get("square_count")
        )
        values = np.asarray(squares, dtype=np.int32)
        if values.ndim != 2 or values.shape[1] != 3:
            invalid_entry_count += len(squares) if isinstance(squares, list) else 1
            continue
        local_indices = values[:, 0]
        elevation_values = values[:, 1]
        terrain_values = values[:, 2]
        sorted_unique = bool(
            np.all((local_indices >= 0) & (local_indices < 4096))
            and np.all(np.diff(local_indices) > 0)
        )
        local_y = local_indices // LOCAL_AXIS
        local_x = local_indices % LOCAL_AXIS
        world_x = int(entry["region_x"]) * LOCAL_AXIS + local_x
        world_y = int(entry["region_y"]) * LOCAL_AXIS + local_y
        raster_rows = WORLD_AXIS - 1 - world_y
        raster_columns = world_x
        already_seen = seen[raster_rows, raster_columns]
        duplicate_count += int(np.count_nonzero(already_seen))
        seen[raster_rows, raster_columns] = True
        physical_matches = (
            land[raster_rows, raster_columns]
            & (elevation[raster_rows, raster_columns].astype(np.int32) == elevation_values)
            & (terrain[raster_rows, raster_columns].astype(np.int32) == terrain_values)
            & (terrain_values >= 1)
            & (terrain_values <= 62)
        )
        value_mismatch_count += int(np.count_nonzero(~physical_matches))
        np.add.at(terrain_counts, terrain_values, 1)
        total_squares += len(squares)
        region_details.append({
            "region_index": entry["region_index"],
            "gzip_signature": gzip_signature,
            "digest_matches": digest_matches,
            "header_matches": header_matches,
            "local_indices_sorted_unique": sorted_unique,
            "square_count": len(squares),
        })

    region_indices = [entry.get("region_index") for entry in region_entries]
    region_coordinates = [(entry.get("region_x"), entry.get("region_y")) for entry in region_entries]
    dictionary_counts = np.array(
        [entry.get("land_cell_count", -1) for entry in terrain_types.get("terrain_types", [])],
        dtype=np.int64,
    )
    results = [
        check("package exposes a bounded-readable top-level /regions array", isinstance(region_entries, list) and bool(region_entries), len(region_entries)),
        check("every gzip region exposes a bounded-readable /squares array", bool(region_details) and all(item["header_matches"] for item in region_details), int(np.count_nonzero([not item["header_matches"] for item in region_details]))),
        check("all region files have gzip signatures and matching checksums", all(item["gzip_signature"] and item["digest_matches"] for item in region_details), int(np.count_nonzero([not (item["gzip_signature"] and item["digest_matches"]) for item in region_details]))),
        check("region indices and coordinates are unique and ordered", region_indices == sorted(region_indices) and len(region_indices) == len(set(region_indices)) == len(set(region_coordinates)), {"regions": len(region_indices), "unique_indices": len(set(region_indices)), "unique_coordinates": len(set(region_coordinates))}),
        check("every /squares array is sparse, sorted, and locally unique", all(item["local_indices_sorted_unique"] for item in region_details), int(np.count_nonzero([not item["local_indices_sorted_unique"] for item in region_details]))),
        check("every JSON square round-trips to the exact elevation and terrain raster values", value_mismatch_count == 0 and invalid_entry_count == 0, {"value_mismatches": value_mismatch_count, "invalid_entries": invalid_entry_count}),
        check("every land square appears exactly once", duplicate_count == 0 and total_squares == int(np.count_nonzero(land)) and np.array_equal(seen, land), {"duplicates": duplicate_count, "json_squares": total_squares, "land_squares": int(np.count_nonzero(land)), "missing": int(np.count_nonzero(land & ~seen))}),
        check("no ocean square is stored", not np.any(seen & ~land) and build.get("ocean_square_count_stored") == 0, int(np.count_nonzero(seen & ~land))),
        check("JSON terrain totals reproduce the terrain dictionary", len(dictionary_counts) == 64 and np.array_equal(dictionary_counts, terrain_counts), {"dictionary_total": int(np.sum(dictionary_counts)) if len(dictionary_counts) == 64 else None, "json_total": int(np.sum(terrain_counts))}),
        check("world geometry is exactly 80 regions by 64 cells by 256 m", world.get("regions_per_axis") == 80 and world.get("cells_per_region_axis") == 64 and world.get("cell_edge_metres") == 256 and world.get("world_side_metres") == 1310720, world),
        check("square payload stores only location plus elevation and terrain", build.get("square_entry_shape") == ["local_index", "elevation_m", "terrain_code"] and world.get("square_physical_fields") == ["elevation_m", "terrain_code"], {"entry": build.get("square_entry_shape"), "physical_fields": world.get("square_physical_fields")}),
        check("plain JSON contract needs neither 64-bit integers nor null/false distinction", build.get("exact_64_bit_integers_required") is False and build.get("null_false_distinction_required") is False, {"int64": build.get("exact_64_bit_integers_required"), "null_false": build.get("null_false_distinction_required")}),
        check("package is bound to frozen QGIS Edition 1", sha256(FROZEN_PROJECT) == freeze.get("qgis_project_sha256") == package.get("frozen_qgis_project_sha256"), package.get("frozen_qgis_project_sha256")),
        check("package and build reports introduce no proper names", terrain_types.get("proper_geographic_names_introduced") is False and build.get("proper_geographic_names_introduced") is False, False),
        check("build report outputs retain their checksums", all((ROOT / relative).is_file() and sha256(ROOT / relative) == expected for relative, expected in build.get("outputs", {}).items()), list(build.get("outputs", {}).keys())),
    ]
    passed = sum(item["pass"] for item in results)
    report = {
        "schema": 1,
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "status": "pass" if passed == len(results) else "fail",
        "checks_passed": passed,
        "checks_total": len(results),
        "checks": results,
        "package": str(PACKAGE.relative_to(ROOT)),
        "package_sha256": sha256(PACKAGE),
        "region_file_count": len(region_entries),
        "land_square_count": total_squares,
        "compressed_region_json_bytes": sum((PACKAGE_DIR / entry["path"]).stat().st_size for entry in region_entries),
    }
    OUTPUT.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    print(json.dumps({"status": report["status"], "checks_passed": passed, "checks_total": len(results), "region_files": len(region_entries), "land_squares": total_squares}, sort_keys=True))
    if report["status"] != "pass":
        raise SystemExit(1)


if __name__ == "__main__":
    main()
