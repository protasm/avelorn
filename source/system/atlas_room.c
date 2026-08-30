int atlas_x;
int atlas_y;
int elevation_m;
int terrain_code;
string terrain_name;
string terrain_display;

void initialize(mixed first_load) {
  atlas_x = -1;
  atlas_y = -1;
  elevation_m = 0;
  terrain_code = 0;
  terrain_name = "unconfigured";
  terrain_display = "Unconfigured";
}

void configure(int x, int y, int elevation, int terrain) {
  atlas_x = x;
  atlas_y = y;
  elevation_m = elevation;
  terrain_code = terrain;
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
  return "The Wilderness";
}

void describe(object viewer) {
  write(short() + "\n");
  write("Terrain: " + terrain_display + "\n");
  write("Elevation: " + elevation_m + " m\n");
  write(terrain_display + " terrain stretches through this part of the wilderness.\n");
  describe_contents(viewer);
  write("Exits: " + exit_text() + "\n");
}

void describe_brief(object viewer) {
  write(short() + "\n");
  write("Terrain: " + terrain_display + "\n");
  write("Elevation: " + elevation_m + " m\n");
  describe_contents(viewer);
  write("Exits: " + exit_text() + "\n");
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
