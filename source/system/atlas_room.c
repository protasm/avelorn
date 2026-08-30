int atlas_x;
int atlas_y;
int elevation_m;
int terrain_code;
int upstream_catchment_ha;
string terrain_name;
string terrain_display;

void initialize(mixed first_load) {
  atlas_x = -1;
  atlas_y = -1;
  elevation_m = 0;
  terrain_code = 0;
  upstream_catchment_ha = 0;
  terrain_name = "unconfigured";
  terrain_display = "Unconfigured";
}

void configure(int x, int y, int elevation, int terrain, int catchment_ha) {
  atlas_x = x;
  atlas_y = y;
  elevation_m = elevation;
  terrain_code = terrain;
  upstream_catchment_ha = catchment_ha;
  terrain_name = jvmud_invoke_lpc_object("system/atlas", "terrain_name", terrain);
  terrain_display = display_terrain(terrain_name);
}

string display_terrain(string identifier) {
  mixed *words;
  string display;
  int index;

  words = jvmud_split_text(identifier, "-");
  display = "";
  for (index = 0; index < jvmud_size(words); index++) {
    if (jvmud_size(display) > 0) {
      display += " ";
    }
    display += jvmud_capitalize_text(words[index]);
  }
  return display;
}

void offer_interactions() {
  jvmud_add_action("north", "north");
  jvmud_add_action("north", "n");
  jvmud_add_action("east", "east");
  jvmud_add_action("east", "e");
  jvmud_add_action("south", "south");
  jvmud_add_action("south", "s");
  jvmud_add_action("west", "west");
  jvmud_add_action("west", "w");
}

string short() {
  return "The Wild";
}

string query_brief_short() {
  return "The Wild (" + atlas_x + ", " + atlas_y + ")";
}

void describe(object viewer) {
  string adjacent;
  string hydrology;

  write(short() + "\n");
  write(terrain_display + " terrain stretches through this part of the wilderness.\n");
  hydrology = catchment_description();
  if (jvmud_size(hydrology) > 0) {
    write(hydrology + "\n");
  }
  adjacent = adjacent_description();
  if (jvmud_size(adjacent) > 0) {
    write(adjacent + "\n");
  }
  describe_contents(viewer);
  write("Exits: " + exit_text() + "\n");
}

void describe_brief(object viewer) {
  write(query_brief_short() + "\n");
  write("Terrain: " + terrain_display + "\n");
  write("Elevation: " + elevation_m + " m\n");
  describe_contents(viewer);
  write("Exits: " + exit_text() + "\n");
}

int query_upstream_catchment_ha() {
  return upstream_catchment_ha;
}

int is_river_terrain() {
  return terrain_code == 3 || terrain_code == 4
      || terrain_code == 52 || terrain_code == 54;
}

string catchment_description() {
  if (!is_river_terrain()) {
    return "";
  }
  if (upstream_catchment_ha < 1000) {
    return "A headwater stream drains the nearby slopes.";
  }
  if (upstream_catchment_ha < 10000) {
    return "A stream gathers the local drainage.";
  }
  if (upstream_catchment_ha < 100000) {
    return "A river drains the surrounding country.";
  }
  return "A major river drains a vast watershed.";
}

string adjacent_description() {
  mixed *coordinates;
  string *directions;
  mixed best_neighbor;
  mixed neighbor;
  string best_direction;
  int best_score;
  int index;
  int score;

  coordinates = ({
      ({ atlas_x, atlas_y + 1 }),
      ({ atlas_x + 1, atlas_y }),
      ({ atlas_x, atlas_y - 1 }),
      ({ atlas_x - 1, atlas_y })
  });
  directions = ({ "north", "east", "south", "west" });
  best_neighbor = 0;
  best_direction = "";
  best_score = 0;
  for (index = 0; index < jvmud_size(coordinates); index++) {
    neighbor = jvmud_invoke_lpc_object(
        "system/atlas", "square", coordinates[index][0], coordinates[index][1]);
    score = adjacent_score(neighbor);
    if (score > best_score) {
      best_neighbor = neighbor;
      best_direction = directions[index];
      best_score = score;
    }
  }
  if (!best_neighbor) {
    return "";
  }
  return adjacent_phrase(best_neighbor, best_direction);
}

int adjacent_score(mixed neighbor) {
  int difference;
  int neighbor_terrain;

  if (!jvmud_is_array(neighbor) || jvmud_size(neighbor) != 4) {
    return 0;
  }
  neighbor_terrain = neighbor[2];
  if ((neighbor_terrain == 1 || neighbor_terrain == 2)
      && terrain_code != 1 && terrain_code != 2) {
    return neighbor_terrain == 1 ? 700 : 650;
  }
  if (neighbor_terrain == 44 && terrain_code != 44) {
    return 600;
  }
  if (is_river_code(neighbor_terrain) && !is_river_terrain()) {
    if (neighbor[3] >= 100000) { return 503; }
    if (neighbor[3] >= 10000) { return 502; }
    if (neighbor[3] >= 1000) { return 501; }
    return 500;
  }
  difference = neighbor[1] - elevation_m;
  if (difference < 0) {
    difference = -difference;
  }
  if (difference >= 64) {
    if (difference > 128) {
      difference = 128;
    }
    return 300 + difference;
  }
  if (neighbor_terrain != terrain_code) {
    return 100;
  }
  return 0;
}

string adjacent_phrase(mixed neighbor, string direction) {
  int difference;
  int neighbor_terrain;
  string neighbor_name;

  neighbor_terrain = neighbor[2];
  if (neighbor_terrain == 1) {
    return "Open water lies to the " + direction + ".";
  }
  if (neighbor_terrain == 2) {
    return "A lake margin lies to the " + direction + ".";
  }
  if (neighbor_terrain == 44) {
    return "A precipice looms to the " + direction + ".";
  }
  if (is_river_code(neighbor_terrain) && !is_river_terrain()) {
    return river_neighbor_phrase(neighbor[3], direction);
  }
  difference = neighbor[1] - elevation_m;
  if (difference >= 128) {
    return "The land rises sharply to the " + direction + ".";
  }
  if (difference <= -128) {
    return "The land falls sharply to the " + direction + ".";
  }
  if (difference >= 64) {
    return "Higher ground rises to the " + direction + ".";
  }
  if (difference <= -64) {
    return "Lower ground falls away to the " + direction + ".";
  }
  neighbor_name = jvmud_invoke_lpc_object(
      "system/atlas", "terrain_name", neighbor_terrain);
  return display_terrain(neighbor_name) + " begins to the " + direction + ".";
}

string river_neighbor_phrase(int catchment_ha, string direction) {
  if (catchment_ha < 1000) {
    return "A headwater stream lies to the " + direction + ".";
  }
  if (catchment_ha < 10000) {
    return "A stream lies to the " + direction + ".";
  }
  if (catchment_ha < 100000) {
    return "A river lies to the " + direction + ".";
  }
  return "A major river lies to the " + direction + ".";
}

int is_river_code(int code) {
  return code == 3 || code == 4 || code == 52 || code == 54;
}

void describe_contents(object viewer) {
  object entity;
  string occupants;
  string items;

  occupants = "";
  items = "";
  entity = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (entity) {
    if (entity != viewer) {
      if (jvmud_method_exists("query_blueprint", entity)
          && jvmud_method_exists("short", entity)) {
        items = append_entry(items, jvmud_invoke_lpc_object(entity, "short"));
      } else if (jvmud_method_exists("query_name", entity)) {
        occupants = append_entry(
            occupants,
            jvmud_invoke_lpc_object(entity, "query_name"));
      }
    }
    entity = jvmud_next_entity_at(entity);
  }
  write("Occupants: " + value_or_none(occupants) + "\n");
  write("Items: " + value_or_none(items) + "\n");
}

string query_area() {
  return "Avelorn Wilderness";
}

string query_environment() {
  return terrain_name;
}

int query_gmcp_id() {
  return 1 + atlas_y * 5120 + atlas_x;
}

mapping query_gmcp_exits() {
  mapping exits;

  exits = ([ ]);
  if (can_walk(atlas_x, atlas_y + 1)) { exits["north"] = room_id(atlas_x, atlas_y + 1); }
  if (can_walk(atlas_x + 1, atlas_y)) { exits["east"] = room_id(atlas_x + 1, atlas_y); }
  if (can_walk(atlas_x, atlas_y - 1)) { exits["south"] = room_id(atlas_x, atlas_y - 1); }
  if (can_walk(atlas_x - 1, atlas_y)) { exits["west"] = room_id(atlas_x - 1, atlas_y); }
  return exits;
}

string query_brief_exits() {
  string exits;

  exits = "";
  if (can_walk(atlas_x, atlas_y + 1)) { exits = append_exit(exits, "n"); }
  if (can_walk(atlas_x + 1, atlas_y)) { exits = append_exit(exits, "e"); }
  if (can_walk(atlas_x, atlas_y - 1)) { exits = append_exit(exits, "s"); }
  if (can_walk(atlas_x - 1, atlas_y)) { exits = append_exit(exits, "w"); }
  return exits;
}

string exit_text() {
  string exits;

  exits = query_brief_exits();
  if (jvmud_size(exits) == 0) {
    return "none";
  }
  return exits;
}

string append_exit(string exits, string direction) {
  if (jvmud_size(exits) == 0) {
    return direction;
  }
  return exits + " " + direction;
}

string append_entry(string entries, string entry) {
  if (jvmud_size(entries) == 0) {
    return entry;
  }
  return entries + ", " + entry;
}

string value_or_none(string value) {
  if (jvmud_size(value) == 0) {
    return "none";
  }
  return value;
}

int room_id(int x, int y) {
  return 1 + y * 5120 + x;
}

int can_walk(int x, int y) {
  return jvmud_invoke_lpc_object(
      "system/atlas", "walking_result", atlas_x, atlas_y, x, y) == 0;
}

int north(mixed ignored) { return walk("north", atlas_x, atlas_y + 1); }
int east(mixed ignored) { return walk("east", atlas_x + 1, atlas_y); }
int south(mixed ignored) { return walk("south", atlas_x, atlas_y - 1); }
int west(mixed ignored) { return walk("west", atlas_x - 1, atlas_y); }

int walk(string direction, int destination_x, int destination_y) {
  int result;

  result = jvmud_invoke_lpc_object(
      "system/atlas",
      "walking_result",
      atlas_x,
      atlas_y,
      destination_x,
      destination_y);
  if (result == 1) {
    write("There is no traversable wilderness square to the " + direction + ".\n");
    return 1;
  }
  if (result == 2) {
    write("Water prevents travel " + direction + ".\n");
    return 1;
  }
  if (result == 3) {
    write("A cliff or precipice prevents travel " + direction + ".\n");
    return 1;
  }
  if (result == 4) {
    write("The elevation change to the " + direction + " is too steep to walk.\n");
    return 1;
  }
  return jvmud_invoke_lpc_object(
      jvmud_current_actor(),
      "travel_to",
      direction,
      jvmud_invoke_lpc_object("system/atlas", "room_path", destination_x, destination_y));
}
