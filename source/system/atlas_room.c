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

  write(short() + "\n");
  write(wilderness_description() + "\n");
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
  return river_description();
}

string wilderness_description() {
  string subject;

  if (is_river_terrain()) {
    return river_description();
  }
  if (terrain_code == 1) {
    return varied_landscape(
        "open water reaches", "open water extends", "still water spreads", "across a broad inland lake");
  }
  if (terrain_code == 2) {
    return varied_landscape(
        "shallows fringe", "reed-fringed water borders", "a shallow lake margin follows", "the edge of an inland lake");
  }
  if (terrain_code == 5 || terrain_code == 6 || terrain_code == 7 || terrain_code == 62) {
    return wetland_description();
  }
  if (terrain_code == 8 || terrain_code == 9 || terrain_code == 55
      || terrain_code == 56 || terrain_code == 57) {
    return coast_description();
  }
  if (terrain_code == 19 || terrain_code == 20) {
    if (terrain_code == 19) { subject = "a forested valley"; }
    if (terrain_code == 20) { subject = "an open valley"; }
    return varied_landscape(
        subject + " lies", subject + " runs", subject + " opens", "between long enclosing slopes");
  }
  if (terrain_code == 21 || terrain_code == 22 || terrain_code == 61) {
    if (terrain_code == 21) { subject = "forested alluvial flats"; }
    if (terrain_code == 22) { subject = "open alluvial flats"; }
    if (terrain_code == 61) { subject = "young alluvial ground"; }
    return cover_description(subject, "along a broad floodplain");
  }
  if (terrain_code >= 23 && terrain_code <= 26) {
    if (terrain_code == 23) { subject = "wooded foothills"; }
    if (terrain_code == 24) { subject = "open woodland foothills"; }
    if (terrain_code == 25) { subject = "scrub-covered foothills"; }
    if (terrain_code == 26) { subject = "grassy foothills"; }
    return varied_landscape(
        subject + " rise", subject + " roll", subject + " continue", "through " + elevation_setting());
  }
  if (terrain_code >= 37 && terrain_code <= 39) {
    if (terrain_code == 37) {
      return varied_landscape(
          "a volcanic plateau stretches", "old volcanic tableland extends",
          "broad volcanic ground lies", "across " + elevation_setting());
    }
    if (terrain_code == 38) { subject = "forest"; }
    if (terrain_code == 39) { subject = "open vegetation"; }
    return varied_landscape(
        subject + " covers", subject + " follows", subject + " clings to", "an old volcanic slope");
  }
  if (terrain_code == 40 || terrain_code == 41) {
    if (terrain_code == 40) { subject = "a forested rocky ridge"; }
    if (terrain_code == 41) { subject = "an open rocky ridge"; }
    return varied_landscape(
        subject + " rises", subject + " runs", subject + " stands", "above the surrounding country");
  }
  if (terrain_code == 42 || terrain_code == 43) {
    if (terrain_code == 42) { subject = "forest"; }
    if (terrain_code == 43) { subject = "sparse open growth"; }
    return varied_landscape(
        subject + " covers", subject + " follows", subject + " clings to", "a steep mountainside");
  }
  if (terrain_code == 44) {
    return varied_landscape(
        "a sheer precipice falls", "a broken cliff drops", "a wall of rock descends", "from " + elevation_setting());
  }
  if (terrain_code >= 45 && terrain_code <= 47) {
    if (terrain_code == 45) { subject = "dry grassland"; }
    if (terrain_code == 46) { subject = "dense shrubland"; }
    if (terrain_code == 47) { subject = "forest"; }
    return cover_description(subject, "within an enclosed basin");
  }
  if (terrain_code == 48 || terrain_code == 49) {
    if (terrain_code == 48) { subject = "low scrub"; }
    if (terrain_code == 49) { subject = "windswept grassland"; }
    return cover_description(subject, "across a high plateau");
  }
  if (terrain_code == 50) {
    return varied_landscape(
        "persistent snowfields cover", "old snow and ice mantle",
        "unbroken snow lies across", elevation_setting());
  }
  if (terrain_code == 51) {
    return varied_landscape(
        "loose talus covers", "angular blocks mantle", "a field of fallen stone crosses", elevation_setting());
  }
  if (terrain_code == 53) {
    return varied_landscape(
        "low ground follows", "a shelving shore borders", "damp basin ground fringes", "the margin of an inland lake");
  }
  if (terrain_code == 58) {
    return varied_landscape(
        "sparse growth struggles", "thin vegetation clings", "bare mineral ground shows", "across exposed ultramafic earth");
  }
  if (terrain_code == 59 || terrain_code == 60) {
    if (terrain_code == 59) { subject = "pale crystalline highlands"; }
    if (terrain_code == 60) { subject = "dark metamorphic highlands"; }
    return varied_landscape(
        subject + " rise", subject + " extend", subject + " dominate", elevation_setting());
  }
  subject = vegetation_subject();
  if (jvmud_size(subject) > 0) {
    return cover_description(subject, "across " + elevation_setting());
  }
  return terrain_display + " shapes " + elevation_setting() + ".";
}

string river_description() {
  string water;

  if (!is_river_terrain()) {
    return "";
  }
  if (upstream_catchment_ha < 1000) {
    water = "a narrow headwater stream";
  } else if (upstream_catchment_ha < 10000) {
    water = "a gathering stream";
  } else if (upstream_catchment_ha < 100000) {
    water = "a broad river";
  } else {
    water = "a great river";
  }
  if (terrain_code == 4) {
    return varied_landscape(
        water + " winds", water + " branches", water + " spreads", "through waterlogged ground");
  }
  if (terrain_code == 52) {
    return varied_landscape(
        water + " runs", water + " cuts", water + " thunders", "at the bottom of a steep-sided gorge");
  }
  if (terrain_code == 54) {
    return varied_landscape(
        "the tidal reaches of " + water + " spread", "salt water meets " + water,
        water + " broadens", "among mudflats and tidal wetlands");
  }
  if (upstream_catchment_ha < 1000) {
    return varied_landscape(
        water + " threads", water + " trickles", water + " descends", "through " + elevation_setting());
  }
  if (upstream_catchment_ha < 10000) {
    return varied_landscape(
        water + " winds", water + " gathers its flow", water + " passes", "through " + elevation_setting());
  }
  if (upstream_catchment_ha < 100000) {
    return varied_landscape(
        water + " crosses", water + " winds through", water + " drains", elevation_setting());
  }
  return varied_landscape(
      water + " commands", water + " crosses", water + " carries distant waters through", elevation_setting());
}

string wetland_description() {
  string subject;

  if (terrain_code == 5) { subject = "salt-touched marshes"; }
  if (terrain_code == 6) { subject = "waterlogged basin marshes"; }
  if (terrain_code == 7) { subject = "dark peat wetlands"; }
  if (terrain_code == 62) { subject = "deep organic wetlands"; }
  return varied_landscape(
      subject + " spread", subject + " extend", subject + " lie", "across " + elevation_setting());
}

string coast_description() {
  if (terrain_code == 8) {
    return varied_landscape(
        "a sandy shore runs", "wind-shaped sand stretches", "a pale beach follows", "along the water's edge");
  }
  if (terrain_code == 9) {
    return varied_landscape(
        "a rocky shore runs", "broken stone meets the water", "wave-worn rock extends", "along the coast");
  }
  if (terrain_code == 55) {
    return varied_landscape(
        "wind-shaped forest crowds", "dense coastal forest follows", "salt-weathered trees cover", "the low coastal ground");
  }
  if (terrain_code == 56) {
    return varied_landscape(
        "coastal scrub covers", "salt-bent shrubs crowd", "low, wind-shaped growth follows", "the ground behind the shore");
  }
  return varied_landscape(
      "dune grassland rolls", "marram-covered dunes extend", "wind-rippled dunes rise", "behind the sandy shore");
}

string vegetation_subject() {
  if (terrain_code == 10) { return "dense warm lowland forest"; }
  if (terrain_code == 11) { return "temperate lowland forest"; }
  if (terrain_code == 12) { return "wet lowland forest"; }
  if (terrain_code == 13) { return "dry open woodland"; }
  if (terrain_code == 14) { return "lowland scrub"; }
  if (terrain_code == 15) { return "lowland grassland"; }
  if (terrain_code == 16) { return "humid forest"; }
  if (terrain_code == 17) { return "dry rolling woodland"; }
  if (terrain_code == 18) { return "rolling grassland"; }
  if (terrain_code == 27) { return "cool montane forest"; }
  if (terrain_code == 28) { return "rain-darkened montane forest"; }
  if (terrain_code == 29) { return "dry montane forest"; }
  if (terrain_code == 30) { return "montane scrub"; }
  if (terrain_code == 31) { return "montane grassland"; }
  if (terrain_code == 32) { return "subalpine scrub"; }
  if (terrain_code == 33) { return "subalpine grassland"; }
  if (terrain_code == 34) { return "alpine grass and herbfield"; }
  if (terrain_code == 35) { return "alpine scree"; }
  if (terrain_code == 36) { return "bare alpine rock"; }
  return "";
}

string cover_description(string subject, string place) {
  return varied_landscape(
      subject + " spreads", subject + " extends", subject + " blankets", place);
}

string varied_landscape(string first, string second, string third, string place) {
  int variant;
  string phrase;

  variant = description_variant();
  if (variant == 0) { phrase = first; }
  if (variant == 1) { phrase = second; }
  if (variant == 2) { phrase = third; }
  return jvmud_capitalize_text(phrase) + " " + place + ".";
}

int description_variant() {
  return (atlas_x * 31 + atlas_y * 17 + terrain_code) % 3;
}

string elevation_setting() {
  if (elevation_m < 50) { return "the low-lying country"; }
  if (elevation_m < 200) { return "the low country"; }
  if (elevation_m < 700) { return "the lower uplands"; }
  if (elevation_m < 1200) { return "the high country"; }
  if (elevation_m < 2000) { return "the alpine heights"; }
  return "the highest reaches";
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
