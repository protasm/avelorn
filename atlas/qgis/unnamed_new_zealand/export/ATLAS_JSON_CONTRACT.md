# Frozen Edition 1 mudlib JSON contract

This package is downstream of `avelorn_physical_edition1.qgz`. It does
not modify or supplement the frozen QGIS source of truth.

## World and region geometry

- world: 80 by 80 regions;
- region: 64 by 64 wilderness squares;
- wilderness square: 256 m by 256 m;
- origin: southwest corner of the frozen world square;
- indices: zero-based and increase east (`x`) and fictional north (`y`).

Ocean-only regions are absent. Ocean squares are absent from land-containing
region files.

## Container shape

`package.json` is an object whose `/regions` member is an array. Each entry
identifies one gzip JSON region file and its checksum.

Each region file is an object whose `/squares` member is an array. JVMud may
read that array structurally with:

`jvmud_read_mudlib_json_array(path, "/squares", offset, count)`

Every square entry is a three-integer array:

`[local_index, elevation_m, terrain_code]`

`local_index` is location, not an additional physical property. It is
`local_y * 64 + local_x`, with both local axes increasing east/north from the
region's southwest corner. The only stored physical properties are the agreed
integer elevation and terrain code.

## JSON type limits

- all payloads use ordinary JSON objects, arrays, strings, integers, and
  booleans;
- no square payload uses `null`;
- no value requires exact 64-bit integer handling;
- terrain codes are 0 through 63, with land using only 1 through 62;
- code 0 is outside/ocean and is never stored as a square;
- code 63 is reserved and is never emitted in Edition 1.

The region files are gzip-compressed, and JVMud detects compression by content
signature rather than filename alone.
