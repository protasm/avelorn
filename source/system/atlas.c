mapping region_cache;
mapping terrain_names;

void initialize(mixed first_load) {
  mapping dictionary;
  mixed *entries;
  mixed entry;
  int index;

  region_cache = ([ ]);
  terrain_names = ([ ]);
  dictionary = jvmud_read_mudlib_json(
      "/atlas/qgis/unnamed_new_zealand/export/edition1_256m_atlas/terrain_types.json");
  if (!jvmud_is_mapping(dictionary) || !jvmud_is_array(dictionary["terrain_types"])) {
    return;
  }
  entries = dictionary["terrain_types"];
  for (index = 0; index < jvmud_size(entries); index++) {
    entry = entries[index];
    if (jvmud_is_mapping(entry)) {
      terrain_names[jvmud_to_string(entry["code"])] = entry["id"];
    }
  }
}

mixed square(int x, int y) {
  mapping region;
  int local_index;

  if (x < 0 || x >= 5120 || y < 0 || y >= 5120) {
    return 0;
  }
  region = load_region(x / 64, y / 64);
  local_index = (y % 64) * 64 + (x % 64);
  if (!jvmud_member(region, jvmud_to_string(local_index))) {
    return 0;
  }
  return region[jvmud_to_string(local_index)];
}

mapping load_region(int region_x, int region_y) {
  mapping region;
  mixed entries;
  mixed entry;
  string cache_key;
  string path;
  int index;

  cache_key = jvmud_format_text("%02d/%02d", region_x, region_y);
  if (jvmud_member(region_cache, cache_key)) {
    return region_cache[cache_key];
  }

  region = ([ ]);
  path = jvmud_format_text(
      "/atlas/qgis/unnamed_new_zealand/export/edition1_256m_atlas/regions/x%02d/r%02d_%02d.json.gz",
      region_x,
      region_x,
      region_y);
  entries = jvmud_read_mudlib_json_array(path, "/squares", 0, 4096);
  if (jvmud_is_array(entries)) {
    for (index = 0; index < jvmud_size(entries); index++) {
      entry = entries[index];
      if (jvmud_is_array(entry) && jvmud_size(entry) == 4) {
        region[jvmud_to_string(entry[0])] = entry;
      }
    }
  }
  region_cache[cache_key] = region;
  return region;
}

string terrain_name(int terrain_code) {
  string name;

  name = terrain_names[jvmud_to_string(terrain_code)];
  if (!name) {
    return "terrain-code-" + terrain_code;
  }
  return name;
}

int upstream_catchment_ha(int x, int y) {
  mixed record;

  record = square(x, y);
  if (!record) {
    return 0;
  }
  return record[3];
}

int walking_result(int from_x, int from_y, int to_x, int to_y) {
  mixed from_square;
  mixed to_square;
  int elevation_change;

  if ((from_x != to_x && from_y != to_y)
      || (from_x == to_x && from_y == to_y)
      || (from_x - to_x > 1) || (to_x - from_x > 1)
      || (from_y - to_y > 1) || (to_y - from_y > 1)) {
    return 1;
  }
  from_square = square(from_x, from_y);
  to_square = square(to_x, to_y);
  if (!from_square || !to_square) {
    return 1;
  }
  if (from_square[2] == 1 || from_square[2] == 2 || from_square[2] == 3
      || to_square[2] == 1 || to_square[2] == 2 || to_square[2] == 3) {
    return 2;
  }
  if (from_square[2] == 44 || to_square[2] == 44) {
    return 3;
  }
  elevation_change = to_square[1] - from_square[1];
  if (elevation_change < 0) {
    elevation_change = -elevation_change;
  }
  if (elevation_change > 128) {
    return 4;
  }
  return 0;
}

string room_path(int x, int y) {
  return jvmud_format_text("place/atlas/x%04d/y%04d", x, y);
}

mixed square_for_path(string path) {
  mixed record;
  string x_text;
  string y_text;
  int x;
  int y;

  if (!path || jvmud_size(path) != 23
      || jvmud_extract_text(path, 0, 12) != "place/atlas/x"
      || jvmud_extract_text(path, 17, 18) != "/y") {
    return 0;
  }
  x_text = jvmud_extract_text(path, 13, 16);
  y_text = jvmud_extract_text(path, 19);
  x = jvmud_to_int(x_text);
  y = jvmud_to_int(y_text);
  if (jvmud_format_text("%04d", x) != x_text
      || jvmud_format_text("%04d", y) != y_text) {
    return 0;
  }
  record = square(x, y);
  if (!record) {
    return 0;
  }
  return ({ x, y, record[1], record[2], record[3] });
}
