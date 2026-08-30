# Edition 1 Physical-Layer Register

This is the admission register for the unnamed, pre-human physical world. It
distinguishes the QGIS source of truth from later land-only mudlib exports. No
civilization, lore, administrative geography, or proper geographic names are
part of Edition 1.

## Edition boundary

- Fictional north is screen-up at QGIS rotation zero.
- The Earth-derived source geometry is rigidly rotated 75 degrees
  counterclockwise before entering the public fictional coordinate system.
- The world is a 1,310,720 m square: 80 by 80 regions, 16,384 m per region,
  64 by 64 analysis cells per region, and 256 m per cell.
- QGIS retains detailed and multi-resolution evidence. The 256 m analysis grid
  is a derived physical-analysis tier, not a replacement for detailed vectors
  or native-resolution source rasters.
- Ocean and shallow-seafloor evidence remain in QGIS. The eventual mudlib JSON
  omits ocean cells and stores elevation, terrain code, and upstream catchment
  area for occupiable land cells.

## Admission states

- **source evidence:** preserved observation or model with documented limits;
- **validated candidate:** internally built and independently checked;
- **Edition 1 canonical:** admitted by the final freeze audit;
- **derived export:** reproducibly compiled after the QGIS edition is frozen.

## Edition 1 candidates

| Physical subject | Edition 1 artifact | Evidence and uncertainty | Admission before freeze |
|---|---|---|---|
| coordinate system and orientation | `WORLD_COORDINATE_CONTRACT.md`, `world_transform_metric.json`, `scope/world_scope.json`, `coordinate_contract_qa.json` | rigid affine transform; fictional Cartesian metre envelope; Earth coordinates are provenance only | validated candidate; 21/21 contract checks |
| world frame and grid | `scope/world_boundary.gpkg`, `scope/world_region_grid.gpkg`, `scope/world_region_grid_summary.json` | exact 80×80 region and 5120×5120 cell address space | validated candidate |
| detailed pre-human land | `terrain/shoreline_edition1_candidate/prehuman_land_world.gpkg`, paired method and uncertainty rasters | 238 mutually corroborated 250 m coastal cells restored to water; no land additions | validated candidate; 17/17 checks |
| observed elevation analysis | `terrain/analysis_256m_edition1/elevation_m.tif`, source and uncertainty rasters | mixed-source regional elevation; preserved observed values; 14 local edge repairs of at most one cell | validated candidate; 18/18 checks |
| hard 256 m coast | `terrain/analysis_256m_edition1/land_mask.tif`, `land_topology_adjustment.tif` | four center-sampled one-cell slivers suppressed without altering the detailed vector | validated candidate; 24 retained island components and no 1–4-cell fragments |
| rivers and lakes | detailed `metric_candidate/world_physical_metric_candidate.gpkg:rivers,lakes` plus `terrain/analysis_256m_edition1/river_mask.tif`, `lake_mask.tif` | source features remain detailed; all 60 represented river features retain continuous 256 m coverage on land | validated candidate |
| conditioned hydrology | `terrain/hydrology_edition1_v2/conditioned_flow_direction.tif`, `water_connector_mask.tif`, `routing_method.tif` | observed elevation unchanged; 752 explicit connector cells; water-direction overrides are separate evidence | validated candidate; 23/23 checks |
| watersheds | `terrain/hydrology_edition1_v2/watershed_basins.tif`, `flow_accumulation_cells.tif` | all 4,051,581 land cells drain to coastal terminals; zero cycles or inland outlets | validated candidate |
| terrain routing repair | `terrain/hydrology_edition1_v2/base_flow_direction_repaired.tif`, `base_watershed_basins_repaired.tif`, `terrain_routing_repair_qa.json` | 4,773 raw cyclic cells repaired; 129 enclosed basins connected through minimum-spill divides | validated supporting evidence; never exported as observed elevation |
| landforms | `terrain/landforms_edition1_v2/landform_class.tif`, slope, relief, and terrain-position rasters | deterministic regional-scale classes on the exact 256 m grid | validated candidate; all land classified |
| geology and substrate | `terrain/geology_edition1_v2/substrate_class_complete_256m.tif`, evidence and inference-distance rasters | 3,884,325 observed cells and 167,256 nearest-unit inferences; maximum inference 37.951 km remains explicit | validated candidate; no uncoded land |
| climate | `terrain/climate_provisional/annual_mean_temperature_c_world_1km.tif`, `annual_precipitation_mm_world_1km.tif`, source manifest and realism QA | broad 1981–2010 climatology sampled downstream; not a 256 m measurement | validated Edition 1 source evidence; realism 6/6 and registration 14/14 |
| potential vegetation | `terrain/vegetation_edition1_v2/potential_vegetation_structure_256m.tif`, uncertainty and coast-distance rasters | ten anonymous structural envelopes; wetlands incorporate admitted drainage; coastal class uses pre-human shore | validated candidate; all land classified |
| soils | `terrain/soils_edition1_v2/soil_regime_256m.tif`, `soil_uncertainty_256m.tif` | twelve broad anonymous regimes; inferred substrate and wet/coastal rules retain elevated uncertainty | validated candidate; all regimes represented and all land classified |
| shallow seafloor | `terrain/seafloor_edition1_candidate/` depth, slope, zone, uncertainty, and shoreline-relationship rasters | inherits public national 250 m compilation limitations; all 238 restored-water cells have direct source depth, with no fallback invention | validated candidate; climate/seafloor audit 14/14 |
| anonymous natural regions | `terrain/natural_regions_edition1_candidate/natural_region_id_256m.tif`, signature and method rasters, dictionary | 5,384 contiguous identifiers; four sub-threshold units are each an entire isolated island | validated candidate; 11/11 checks |

## Cross-layer gates

- The fresh 256 m physical foundation passes 18/18 independent checks.
- Hydrology and watersheds pass 23/23 independent topology, realism, digest,
  and QGIS-binding checks.
- Landforms, substrate, vegetation, and soils pass 17/17 cross-layer checks in
  QGIS v013.
- Anonymous natural regions pass 11/11 topology and evidence checks.
- Climate and reconciled seafloor pass 14/14 combined checks.
- All named QGIS evidence groups are hidden by default so the visual atlas
  remains readable while retaining full audit complexity.

## Accepted Edition 1 limitations

Edition 1 is a plausible regional physical world, not a blade-of-grass survey.
The following limitations remain recorded without blocking the freeze:

- regional elevation inherits mixed-source and surface-model limitations;
- hydrologic direction is a connectivity model, not a claim that every 256 m
  observed elevation step descends;
- geology is regional-scale and includes explicit nearest-unit inference;
- climate is kilometre-scale climatology;
- vegetation and soils are broad anonymous physical envelopes;
- seafloor inherits older national compilation and survey-density limits;
- the 256 m land tier deliberately suppresses four rasterization slivers while
  the detailed pre-human vector remains preserved.

These are candidates for later editions, not reasons to keep Edition 1 open
indefinitely.

## Downstream boundary

The old `export/provisional_256m_atlas_v001/` package is retained only as a
historical playable prototype. It is not an input to any fresh Edition 1
physical layer. A final mudlib package must be generated anew from the frozen
QGIS edition and may store only the agreed land-cell elevation, terrain code,
and upstream catchment area.
