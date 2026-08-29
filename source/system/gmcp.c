mapping learned_exits;

void initialize(mixed first_load) {
  learned_exits = ([ ]);
  seed_exit("place/ashenwatch/approach", "east", "place/blackstone/standing_stones");
  seed_exit("place/ashenwatch/approach", "west", "place/ashenwatch/expedition_camp");
  seed_exit("place/ashenwatch/barracks", "east", "place/ashenwatch/outer_court");
  seed_exit("place/ashenwatch/chapel", "north", "place/ashenwatch/outer_court");
  seed_exit("place/ashenwatch/crown_lantern", "north", "place/world/r00000");
  seed_exit("place/ashenwatch/crown_lantern", "south", "place/ashenwatch/lantern_crypt");
  seed_exit("place/ashenwatch/east_tower", "west", "place/ashenwatch/great_hall");
  seed_exit("place/ashenwatch/expedition_camp", "west", "place/ashenwatch/lower_gate");
  seed_exit("place/ashenwatch/great_hall", "north", "place/ashenwatch/underkeep");
  seed_exit("place/ashenwatch/great_hall", "south", "place/ashenwatch/outer_court");
  seed_exit("place/ashenwatch/great_hall", "west", "place/ashenwatch/west_tower");
  seed_exit("place/ashenwatch/lantern_crypt", "west", "place/ashenwatch/ward_antechamber");
  seed_exit("place/ashenwatch/lower_gate", "west", "place/ashenwatch/outer_court");
  seed_exit("place/ashenwatch/underkeep", "down", "place/ashenwatch/ward_antechamber");
  seed_exit("place/blackstone/flooded_gallery", "west", "place/blackstone/wardwork_threshold");
  seed_exit("place/blackstone/old_armory", "east", "place/blackstone/wardwork_threshold");
  seed_exit("place/blackstone/shepherd_hut", "west", "place/blackstone/upland_trail");
  seed_exit("place/blackstone/standing_stones", "north", "place/blackstone/wardwork_entrance");
  seed_exit("place/blackstone/standing_stones", "south", "place/blackstone/upland_trail");
  seed_exit("place/blackstone/upland_trail", "south", "place/merewatch/upland_gate");
  seed_exit("place/blackstone/upland_trail", "west", "place/merewatch/reed_shrine");
  seed_exit("place/blackstone/ward_chamber", "south", "place/blackstone/wardwork_threshold");
  seed_exit("place/blackstone/wardwork_entrance", "down", "place/blackstone/wardwork_threshold");
  seed_exit("place/brindleford/cellar_landing", "east", "place/brindleford/grain_cellar");
  seed_exit("place/brindleford/cellar_landing", "west", "place/brindleford/pump_room");
  seed_exit("place/brindleford/cellar_landing", "up", "place/brindleford/mill_yard");
  seed_exit("place/brindleford/east_road", "east", "place/brindleford/old_bridge");
  seed_exit("place/brindleford/east_road", "west", "place/brindleford/mill_road");
  seed_exit("place/brindleford/lantern_house", "south", "place/brindleford/village_green");
  seed_exit("place/brindleford/market", "east", "place/brindleford/mill_road");
  seed_exit("place/brindleford/market", "west", "place/brindleford/village_green");
  seed_exit("place/brindleford/mill_road", "north", "place/brindleford/mill_yard");
  seed_exit("place/brindleford/old_bridge", "east", "place/lantern_road/toll_meadow");
  seed_exit("place/brindleford/reeves_hall", "east", "place/brindleford/village_green");
  seed_exit("place/brindleford/shrine", "north", "place/brindleford/village_green");
  seed_exit("place/greyhaven/archive_court", "east", "place/greyhaven/heron_fountain");
  seed_exit("place/greyhaven/company_hall", "north", "place/greyhaven/watch_barracks");
  seed_exit("place/greyhaven/company_hall", "east", "place/greyhaven/smith_lane");
  seed_exit("place/greyhaven/company_hall", "west", "place/greyhaven/heron_fountain");
  seed_exit("place/greyhaven/gate_square", "east", "place/greyhaven/market_cross");
  seed_exit("place/greyhaven/gate_square", "west", "place/greyhaven/west_gate");
  seed_exit("place/greyhaven/guild_row", "west", "place/greyhaven/market_cross");
  seed_exit("place/greyhaven/heron_fountain", "north", "place/greyhaven/temple_court");
  seed_exit("place/greyhaven/heron_fountain", "south", "place/greyhaven/market_cross");
  seed_exit("place/greyhaven/lantern_tower", "south", "place/greyhaven/temple_court");
  seed_exit("place/greyhaven/market_cross", "south", "place/greyhaven/river_quay");
  seed_exit("place/greyhaven/north_gate", "north", "place/north_road/patrol_crossing");
  seed_exit("place/greyhaven/north_gate", "south", "place/greyhaven/watch_barracks");
  seed_exit("place/greyhaven/west_gate", "west", "place/lantern_road/greyhaven_approach");
  seed_exit("place/lantern_road/birch_copse", "south", "place/lantern_road/orchard_lane");
  seed_exit("place/lantern_road/crown_shelter", "east", "place/lantern_road/orchard_lane");
  seed_exit("place/lantern_road/crown_shelter", "west", "place/lantern_road/royal_waystone");
  seed_exit("place/lantern_road/greyhaven_approach", "west", "place/lantern_road/westward_rise");
  seed_exit("place/lantern_road/lamplighter_post", "east", "place/lantern_road/westward_rise");
  seed_exit("place/lantern_road/lamplighter_post", "west", "place/lantern_road/orchard_lane");
  seed_exit("place/lantern_road/riverside_path", "north", "place/lantern_road/royal_waystone");
  seed_exit("place/lantern_road/royal_waystone", "west", "place/lantern_road/toll_meadow");
  seed_exit("place/merewatch/lakeside", "east", "place/merewatch/mere_square");
  seed_exit("place/merewatch/mere_square", "north", "place/merewatch/upland_gate");
  seed_exit("place/merewatch/mere_square", "east", "place/merewatch/warden_hall");
  seed_exit("place/merewatch/mere_square", "south", "place/merewatch/south_gate");
  seed_exit("place/merewatch/south_gate", "south", "place/north_road/merewatch_road");
  seed_exit("place/north_road/abandoned_post", "north", "place/north_road/merewatch_road");
  seed_exit("place/north_road/abandoned_post", "south", "place/north_road/patrol_crossing");
  seed_exit("place/north_road/patrol_crossing", "east", "place/north_road/shepherd_fields");
}

void seed_exit(string origin, string direction, string destination) {
  string reverse;

  remember_exit(origin, direction, destination);
  reverse = reverse_direction(direction);
  if (jvmud_size(reverse) > 0) {
    remember_exit(destination, reverse, origin);
  }
}

int room_id(string path) {
  string *static_rooms;
  string number;
  int index;

  if (jvmud_size(path) == 18
      && jvmud_extract_text(path, 0, 12) == "place/world/r") {
    number = jvmud_extract_text(path, 13);
    index = jvmud_to_int(number);
    if (index >= 0 && index < 99935 && jvmud_format_text("%05d", index) == number) {
      return 66 + index;
    }
  }

  static_rooms = ({
    "place/ashenwatch/approach",
    "place/ashenwatch/barracks",
    "place/ashenwatch/chapel",
    "place/ashenwatch/crown_lantern",
    "place/ashenwatch/east_tower",
    "place/ashenwatch/expedition_camp",
    "place/ashenwatch/great_hall",
    "place/ashenwatch/lantern_crypt",
    "place/ashenwatch/lower_gate",
    "place/ashenwatch/outer_court",
    "place/ashenwatch/underkeep",
    "place/ashenwatch/ward_antechamber",
    "place/ashenwatch/west_tower",
    "place/blackstone/flooded_gallery",
    "place/blackstone/old_armory",
    "place/blackstone/shepherd_hut",
    "place/blackstone/standing_stones",
    "place/blackstone/upland_trail",
    "place/blackstone/ward_chamber",
    "place/blackstone/wardwork_entrance",
    "place/blackstone/wardwork_threshold",
    "place/brindleford/cellar_landing",
    "place/brindleford/east_road",
    "place/brindleford/grain_cellar",
    "place/brindleford/lantern_house",
    "place/brindleford/market",
    "place/brindleford/mill_road",
    "place/brindleford/mill_yard",
    "place/brindleford/old_bridge",
    "place/brindleford/pump_room",
    "place/brindleford/reeves_hall",
    "place/brindleford/shrine",
    "place/brindleford/village_green",
    "place/greyhaven/archive_court",
    "place/greyhaven/company_hall",
    "place/greyhaven/gate_square",
    "place/greyhaven/guild_row",
    "place/greyhaven/heron_fountain",
    "place/greyhaven/lantern_tower",
    "place/greyhaven/market_cross",
    "place/greyhaven/north_gate",
    "place/greyhaven/river_quay",
    "place/greyhaven/smith_lane",
    "place/greyhaven/temple_court",
    "place/greyhaven/watch_barracks",
    "place/greyhaven/west_gate",
    "place/lantern_road/birch_copse",
    "place/lantern_road/crown_shelter",
    "place/lantern_road/greyhaven_approach",
    "place/lantern_road/lamplighter_post",
    "place/lantern_road/orchard_lane",
    "place/lantern_road/riverside_path",
    "place/lantern_road/royal_waystone",
    "place/lantern_road/toll_meadow",
    "place/lantern_road/westward_rise",
    "place/merewatch/lakeside",
    "place/merewatch/mere_square",
    "place/merewatch/reed_shrine",
    "place/merewatch/south_gate",
    "place/merewatch/upland_gate",
    "place/merewatch/warden_hall",
    "place/north_road/abandoned_post",
    "place/north_road/merewatch_road",
    "place/north_road/patrol_crossing",
    "place/north_road/shepherd_fields"
  });
  index = 0;
  while (index < jvmud_size(static_rooms)) {
    if (static_rooms[index] == path) {
      return index + 1;
    }
    index += 1;
  }
  return 0;
}

void record_travel(object origin, string direction, string destination) {
  string origin_path;
  string reverse;

  if (!origin || room_id(destination) <= 0) {
    return;
  }
  origin_path = jvmud_lpc_object_id(origin);
  if (room_id(origin_path) <= 0) {
    return;
  }
  remember_exit(origin_path, direction, destination);
  reverse = reverse_direction(direction);
  if (jvmud_size(reverse) > 0) {
    remember_exit(destination, reverse, origin_path);
  }
}

void remember_exit(string origin, string direction, string destination) {
  mapping exits;

  exits = learned_exits[origin];
  if (!exits) {
    exits = ([ ]);
    learned_exits[origin] = exits;
  }
  exits[direction] = room_id(destination);
}

string reverse_direction(string direction) {
  if (direction == "north") { return "south"; }
  if (direction == "east") { return "west"; }
  if (direction == "south") { return "north"; }
  if (direction == "west") { return "east"; }
  if (direction == "up") { return "down"; }
  if (direction == "down") { return "up"; }
  return "";
}

void send_room(object player) {
  object place;
  string path;
  string area;
  string environment;
  mapping exits;
  mapping info;

  if (!jvmud_gmcp_enabled(player)) {
    return;
  }
  place = jvmud_entity_location(player);
  if (!place) {
    return;
  }
  path = jvmud_lpc_object_id(place);
  if (room_id(path) <= 0) {
    return;
  }
  area = area_name(path, place);
  environment = environment_name(path, place);
  if (jvmud_method_exists("query_gmcp_exits", place)) {
    exits = jvmud_invoke_lpc_object(place, "query_gmcp_exits");
  } else {
    exits = learned_exits[path];
    if (!exits) {
      exits = ([ ]);
    }
  }
  info = ([
    "num": room_id(path),
    "name": jvmud_invoke_lpc_object(place, "short"),
    "area": area,
    "environment": environment,
    "details": ({ }),
    "exits": exits
  ]);
  jvmud_send_gmcp("Room.Info", info);
}

string area_name(string path, object place) {
  if (jvmud_method_exists("query_area", place)) {
    return jvmud_invoke_lpc_object(place, "query_area");
  }
  if (jvmud_extract_text(path, 0, 16) == "place/brindleford") { return "Brindleford"; }
  if (jvmud_extract_text(path, 0, 14) == "place/greyhaven") { return "Greyhaven"; }
  if (jvmud_extract_text(path, 0, 15) == "place/ashenwatch") { return "Ashenwatch"; }
  if (jvmud_extract_text(path, 0, 14) == "place/blackstone") { return "Blackstone Uplands"; }
  if (jvmud_extract_text(path, 0, 14) == "place/merewatch") { return "Merewatch"; }
  if (jvmud_extract_text(path, 0, 17) == "place/lantern_road") { return "Lantern Road"; }
  if (jvmud_extract_text(path, 0, 15) == "place/north_road") { return "North Road"; }
  return "Avelorn";
}

string environment_name(string path, object place) {
  if (jvmud_method_exists("query_environment", place)) {
    return jvmud_invoke_lpc_object(place, "query_environment");
  }
  if (jvmud_extract_text(path, 0, 15) == "place/ashenwatch") { return "castle"; }
  if (jvmud_extract_text(path, 0, 17) == "place/lantern_road"
      || jvmud_extract_text(path, 0, 15) == "place/north_road") { return "road"; }
  if (jvmud_extract_text(path, 0, 14) == "place/blackstone") { return "uplands"; }
  return "settlement";
}
