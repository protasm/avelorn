// Loaded by acceptance tests to exercise the same LPC definitions used by players.
mapping verify() {
  mapping areas;
  mapping places;
  mapping rooms;
  mapping regions;
  mapping area;
  mapping room;
  mixed *keys;
  string path;
  string id;
  int index;
  int city;
  int settlement;
  int site;
  int details;

  areas = jvmud_invoke_lpc_object("system/world", "section", "areas");
  places = jvmud_invoke_lpc_object("system/world", "section", "places");
  regions = jvmud_invoke_lpc_object("system/world", "section", "regions");
  rooms = ([ ]);
  keys = jvmud_mapping_keys(places);
  for (index = 0; index < jvmud_size(keys); index += 1) {
    path = keys[index];
    room = jvmud_invoke_lpc_object(path, "definition");
    require(jvmud_is_mapping(room) && jvmud_size(room["name"])
        && jvmud_is_mapping(room["exits"]) && jvmud_size(room["details"]),
        "Incomplete authored Place " + path);
    rooms[path] = room;
    details += jvmud_size(room["details"]);
  }
  keys = jvmud_mapping_keys(areas);
  for (index = 0; index < jvmud_size(keys); index += 1) {
    id = keys[index];
    area = areas[id];
    verify_area(id, area, regions, rooms);
    if (area["kind"] == "city") { city += 1; }
    else if (area["kind"] == "site") { site += 1; }
    else { settlement += 1; }
  }
  verify_exits(rooms);
  verify_trails(areas);
  verify_routes(areas, places, rooms);
  path = jvmud_invoke_lpc_object("system/world", "starting_place");
  require(rooms[path] && rooms[path]["safe"], "The starting Place needs a safe hearth");
  verify_connections(places[path]["area"], places, areas);
  verify_quests(rooms);
  return ([
    "regions": jvmud_size(regions), "cities": city, "settlements": settlement,
    "sites": site, "rooms": jvmud_size(rooms), "details": details
  ]);
}

void require(int condition, string problem) {
  if (!condition) {
    jvmud_raise_error(problem + ".");
  }
}

void verify_area(string id, mapping area, mapping regions, mapping rooms) {
  mapping region;
  mixed *point;
  string entrance;
  string atlas_path;
  int x;
  int y;

  point = area["anchor"];
  x = point[0];
  y = point[1];
  entrance = area["entrance"];
  atlas_path = jvmud_invoke_lpc_object("system/atlas", "room_path", x, y);
  region = regions[area["nation"]];
  require(region && historical_nation(x, y) == region["historicalCode"],
      id + " falls outside its historical nation");
  require(rooms[entrance]["exits"]["out"] == atlas_path
      && jvmud_invoke_lpc_object("system/world", "atlas_entrance", atlas_path) == entrance,
      id + " has an invalid atlas entrance");
  require(walks(x, y, x + 1, y) || walks(x, y, x - 1, y)
      || walks(x, y, x, y + 1) || walks(x, y, x, y - 1),
      id + " has no walkable approach");
}

int historical_nation(int x, int y) {
  string root;
  string package_path;
  string region_path;
  mapping descriptor;
  mixed region;
  mixed *runs;
  int local;
  int index;

  root = "/atlas/political/pre-conquest/";
  descriptor = jvmud_read_mudlib_json(root + "working-borders.json");
  package_path = descriptor["package"]["path"];
  require(jvmud_size(package_path) > 12, "Invalid historical atlas descriptor");
  root += jvmud_extract_text(package_path, 0, jvmud_size(package_path) - 13);
  region_path = jvmud_format_text("regions/x%02d/r%02d_%02d.json.gz", x / 64, x / 64, y / 64);
  region = jvmud_read_mudlib_json(root + region_path);
  if (!jvmud_is_mapping(region)) { return 0; }
  local = (y % 64) * 64 + x % 64;
  runs = region["overrides"];
  for (index = 0; index < jvmud_size(runs); index += 1) {
    if (runs[index][0] == local) { return runs[index][1]; }
  }
  runs = region["coverage_runs"];
  for (index = 0; index < jvmud_size(runs); index += 1) {
    if (local >= runs[index][0] && local < runs[index][0] + runs[index][1]) {
      return region["default_nation"];
    }
  }
  return 0;
}

void verify_exits(mapping rooms) {
  mixed *paths;
  mixed *destinations;
  mixed destination;
  string path;
  int index;
  int exit_index;

  paths = jvmud_mapping_keys(rooms);
  for (index = 0; index < jvmud_size(paths); index += 1) {
    path = paths[index];
    destinations = jvmud_mapping_values(rooms[path]["exits"]);
    for (exit_index = 0; exit_index < jvmud_size(destinations); exit_index += 1) {
      destination = destinations[exit_index];
      if (rooms[destination]) {
        require(jvmud_member(jvmud_mapping_values(rooms[destination]["exits"]), path) >= 0,
            path + " has no return from " + destination);
      } else {
        require(jvmud_invoke_lpc_object("system/atlas", "square_for_path", destination)
            && jvmud_invoke_lpc_object("system/world", "atlas_entrance", destination) == path,
            path + " has an invalid off-map exit " + destination);
      }
    }
  }
}

int walks(int x, int y, int next_x, int next_y) {
  return jvmud_invoke_lpc_object("system/atlas", "walking_result", x, y, next_x, next_y) == 0;
}

void verify_trails(mapping areas) {
  mapping trails;
  mapping trail;
  mapping offsets;
  mixed *keys;
  mixed *point;
  mixed *target;
  mixed *steps;
  mixed *offset;
  int x;
  int y;
  int index;
  int step;

  offsets = ([ "north": ({ 0, 1 }), "east": ({ 1, 0 }),
      "south": ({ 0, -1 }), "west": ({ -1, 0 }) ]);
  trails = jvmud_invoke_lpc_object("system/world", "section", "trails");
  keys = jvmud_mapping_keys(trails);
  for (index = 0; index < jvmud_size(keys); index += 1) {
    trail = trails[keys[index]];
    require(areas[trail["from"]] && areas[trail["to"]], "Unknown endpoint for " + keys[index]);
    point = areas[trail["from"]]["anchor"];
    target = areas[trail["to"]]["anchor"];
    x = point[0];
    y = point[1];
    steps = trail["steps"];
    require(jvmud_size(steps) > 0, "Empty walking route " + keys[index]);
    for (step = 0; step < jvmud_size(steps); step += 1) {
      offset = offsets[steps[step]];
      require(offset && walks(x, y, x + offset[0], y + offset[1]),
          "Blocked walking route " + keys[index] + " at " + x + ", " + y);
      x += offset[0];
      y += offset[1];
    }
    require(x == target[0] && y == target[1], "Wrong destination for " + keys[index]);
  }
}

void verify_routes(mapping areas, mapping places, mapping rooms) {
  mapping routes;
  mapping route;
  mapping reverse;
  mixed *keys;
  string from;
  string to;
  int index;

  routes = jvmud_invoke_lpc_object("system/world", "section", "routes");
  reverse = ([ ]);
  keys = jvmud_mapping_keys(routes);
  for (index = 0; index < jvmud_size(keys); index += 1) {
    route = routes[keys[index]];
    from = route["from"];
    to = route["to"];
    require(places[from] && places[to] && from != to, "Invalid route " + keys[index]);
    require(areas[places[from]["area"]]["entrance"] == from
        && jvmud_member(rooms[from]["services"], "transit") >= 0
        && areas[places[to]["area"]]["entrance"] == to
        && jvmud_member(rooms[to]["services"], "transit") >= 0,
        "Invalid terminal for " + keys[index]);
    require(route["duration"] > 0 && route["duration"] <= 120 && route["fare"] >= 0,
        "Invalid duration or fare for " + keys[index]);
    require(!reverse[from + ":" + to], "Ambiguous route from " + from + " to " + to);
    reverse[from + ":" + to] = 1;
  }
  for (index = 0; index < jvmud_size(keys); index += 1) {
    route = routes[keys[index]];
    require(reverse[route["to"] + ":" + route["from"]], "No return route for " + keys[index]);
  }
}

void verify_connections(string start, mapping places, mapping areas) {
  mapping routes;
  mapping trails;
  mapping free;
  mapping connected;
  mapping route;
  mapping reachable;
  mixed *routes_ids;
  mixed *trail_ids;
  mixed *area_ids;
  string from;
  string to;
  int index;
  int other;

  routes = jvmud_invoke_lpc_object("system/world", "section", "routes");
  trails = jvmud_invoke_lpc_object("system/world", "section", "trails");
  free = ([ ]);
  connected = ([ ]);
  routes_ids = jvmud_mapping_keys(routes);
  for (index = 0; index < jvmud_size(routes_ids); index += 1) {
    route = routes[routes_ids[index]];
    if (!route["fare"]) {
      from = places[route["from"]]["area"];
      to = places[route["to"]]["area"];
      add_connection(free, from, to);
      add_connection(connected, from, to);
    }
  }
  trail_ids = jvmud_mapping_keys(trails);
  for (index = 0; index < jvmud_size(trail_ids); index += 1) {
    route = trails[trail_ids[index]];
    add_connection(connected, route["from"], route["to"]);
    add_connection(connected, route["to"], route["from"]);
  }
  area_ids = jvmud_mapping_keys(areas);
  reachable = distances(connected, start);
  for (index = 0; index < jvmud_size(area_ids); index += 1) {
    from = area_ids[index];
    require(jvmud_member(reachable, from), "No free approach to " + from);
    require(jvmud_member(distances(connected, from), start), "No free return from " + from);
    if (from == start || areas[from]["kind"] == "city") {
      reachable = distances(free, from);
      for (other = 0; other < jvmud_size(area_ids); other += 1) {
        to = area_ids[other];
        if (to == start || areas[to]["kind"] == "city") {
          require(jvmud_member(reachable, to) && reachable[to] <= 3,
              "No three-leg free passage from " + from + " to " + to);
        }
      }
      reachable = distances(connected, start);
    }
  }
}

void add_connection(mapping network, string from, string to) {
  mixed *links;

  links = network[from];
  network[from] = links ? links + ({ to }) : ({ to });
}

mapping distances(mapping network, string start) {
  mapping seen;
  mixed *queue;
  mixed *links;
  string from;
  string to;
  int index;
  int neighbor;

  seen = ([ start: 0 ]);
  queue = ({ start });
  for (index = 0; index < jvmud_size(queue); index += 1) {
    from = queue[index];
    links = network[from];
    for (neighbor = 0; neighbor < jvmud_size(links); neighbor += 1) {
      to = links[neighbor];
      if (!jvmud_member(seen, to)) {
        seen[to] = seen[from] + 1;
        queue += ({ to });
      }
    }
  }
  return seen;
}

void verify_quests(mapping rooms) {
  mapping issuers;
  mapping actions;
  mapping defeats;
  mapping resident;
  mapping quest;
  mapping room;
  mixed *rooms_ids;
  mixed *quest_ids;
  mixed *entries;
  string id;
  int index;
  int item;
  int repeatable;

  issuers = ([ ]);
  actions = ([ ]);
  defeats = ([ ]);
  rooms_ids = jvmud_mapping_keys(rooms);
  for (index = 0; index < jvmud_size(rooms_ids); index += 1) {
    room = rooms[rooms_ids[index]];
    entries = room["quests"];
    for (item = 0; item < jvmud_size(entries); item += 1) {
      require(jvmud_member(room["services"], "noticeboard") >= 0, "Missing quest noticeboard");
      record_issuer(issuers, entries[item]);
    }
    entries = room["npcs"];
    for (item = 0; item < jvmud_size(entries); item += 1) {
      resident = jvmud_invoke_lpc_object("system/creatures", "definition", entries[item]);
      require(resident != 0, "Unknown resident " + entries[item]);
      if (jvmud_size(resident["quest"])) { record_issuer(issuers, resident["quest"]); }
      if (jvmud_size(resident["defeatTag"])) { defeats[resident["defeatTag"]] = 1; }
    }
    if (room["action"]) { actions[room["action"]["tag"]] = 1; }
    entries = room["actions"];
    for (item = 0; item < jvmud_size(entries); item += 1) {
      actions[entries[item]["tag"]] = 1;
    }
  }
  quest_ids = jvmud_invoke_lpc_object("system/quests", "ids");
  for (index = 0; index < jvmud_size(quest_ids); index += 1) {
    id = quest_ids[index];
    quest = jvmud_invoke_lpc_object("system/quests", "definition", id);
    require(issuers[id], "No issuer for " + id);
    require(quest["level"] > 0
        && quest["level"] <= jvmud_invoke_lpc_object("system/progression", "cap") && quest["count"] > 0
        && quest["xp"] >= 0 && quest["copper"] >= 0, "Invalid progression or reward for " + id);
    require(verify_objectives(id, quest, "action", actions)
        + verify_objectives(id, quest, "defeat", defeats) == quest["count"],
        "Wrong objective count for " + id);
    verify_prerequisites(id, quest);
    if (quest["repeatable"]) { repeatable += 1; }
  }
  require(repeatable >= 10 && jvmud_size(quest_ids) - repeatable >= 40, "Missing launch quests");
}

void record_issuer(mapping issuers, string id) {
  require(jvmud_invoke_lpc_object("system/quests", "known", id) && !issuers[id],
      "Unknown or duplicate issued quest " + id);
  issuers[id] = 1;
}

int verify_objectives(string id, mapping quest, string kind, mapping known) {
  mixed *tags;
  string single;
  int index;
  int count;

  single = quest[kind + "_tag"];
  if (jvmud_size(single)) {
    require(known[single], "No " + kind + " objective " + single + " for " + id);
    count += 1;
  }
  tags = quest[kind + "_tags"];
  for (index = 0; index < jvmud_size(tags); index += 1) {
    require(known[tags[index]], "No " + kind + " objective " + tags[index] + " for " + id);
    count += 1;
  }
  return count;
}

void verify_prerequisites(string id, mapping quest) {
  mapping seen;
  string prerequisite;

  seen = ([ id: 1 ]);
  prerequisite = quest["prerequisite"];
  while (jvmud_size(prerequisite)) {
    require(!seen[prerequisite], "Cyclic prerequisites for " + id);
    seen[prerequisite] = 1;
    quest = jvmud_invoke_lpc_object("system/quests", "definition", prerequisite);
    require(quest != 0, "Unknown prerequisite " + prerequisite + " for " + id);
    prerequisite = quest["prerequisite"];
  }
}
