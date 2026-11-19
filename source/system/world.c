nosave mapping content;
nosave mapping entrances;
nosave mapping room_ids;

void initialize(mixed first_load) {
  mixed *registered;
  int index;

  content = ([
    "start": jvmud_invoke_lpc_object("area/registry", "starting_place"),
    "regions": jvmud_invoke_lpc_object("area/registry", "regions"),
    "areas": ([ ]),
    "places": ([ ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([ ]),
    "sites": ([ ])
  ]);
  entrances = ([ ]);
  room_ids = ([ ]);
  registered = jvmud_invoke_lpc_object("area/registry", "areas");
  for (index = 0; index < jvmud_size(registered); index += 1) {
    load_area(registered[index]);
  }
  if (jvmud_size(registered) && !place(content["start"])) {
    jvmud_raise_error("Unregistered starting place " + content["start"] + ".");
  }
}

void load_area(string id) {
  mapping entry;
  mapping rooms;
  mapping places;
  mixed *paths;
  mixed *room;
  mixed *anchor;
  string path;
  string number;
  int index;

  entry = jvmud_invoke_lpc_object("area/" + id + "/area", "definition");
  if (!jvmud_is_mapping(entry) || !jvmud_is_mapping(entry["rooms"]) || area(id)) {
    jvmud_raise_error("Invalid or duplicate area registration " + id + ".");
  }
  content["areas"][id] = entry;
  rooms = entry["rooms"];
  places = section("places");
  paths = jvmud_mapping_keys(rooms);
  for (index = 0; index < jvmud_size(paths); index += 1) {
    path = paths[index];
    room = rooms[path];
    if (!jvmud_is_array(room) || jvmud_size(room) != 2 || !jvmud_is_int(room[0])
        || room[0] < 30000000 || !jvmud_is_string(room[1])) {
      jvmud_raise_error("Invalid room registration " + path + " in " + id + ".");
    }
    number = jvmud_to_string(room[0]);
    if (places[path] || room_ids[number]) {
      jvmud_raise_error("Duplicate Place path or GMCP number at " + path + ".");
    }
    places[path] = ([ "area": id, "id": room[0], "environment": room[1] ]);
    room_ids[number] = path;
  }
  anchor = entry["anchor"];
  if (!jvmud_is_array(anchor) || jvmud_size(anchor) != 2 || !rooms[entry["entrance"]]) {
    jvmud_raise_error("Area " + id + " needs an atlas anchor and a registered entrance.");
  }
  path = jvmud_invoke_lpc_object("system/atlas", "room_path", anchor[0], anchor[1]);
  if (entrances[path]) {
    jvmud_raise_error("Duplicate atlas anchor " + path + ".");
  }
  entrances[path] = entry["entrance"];
  merge_section("routes", entry["routes"], id);
  merge_section("trails", entry["trails"], id);
  merge_section("landmarks", entry["landmarks"], id);
  if (entry["site"]) {
    content["sites"][id] = entry["site"];
  }
}

void merge_section(string name, mixed additions, string area_id) {
  mapping entries;
  mixed *names;
  string id;
  int index;

  if (!additions) { return; }
  if (!jvmud_is_mapping(additions)) {
    jvmud_raise_error("Invalid " + name + " in " + area_id + ".");
  }
  entries = section(name);
  names = jvmud_mapping_keys(additions);
  for (index = 0; index < jvmud_size(names); index += 1) {
    id = names[index];
    if (jvmud_member(entries, id)) {
      jvmud_raise_error("Duplicate " + name + " id " + id + " in " + area_id + ".");
    }
    entries[id] = additions[id];
  }
}

mapping section(string name) {
  mixed value;

  value = content[name];
  return jvmud_is_mapping(value) ? value : ([ ]);
}

mixed place(string path) {
  return section("places")[path];
}

mixed area(string id) {
  return section("areas")[id];
}

mixed landmark(string path) {
  return section("landmarks")[path];
}

string join_names(mixed *names, string separator) {
  string text;
  int index;

  text = "";
  for (index = 0; index < jvmud_size(names); index += 1) {
    if (index) {
      text += separator;
    }
    text += names[index];
  }
  return text;
}

string direction_name(string direction) {
  if (direction == "n") { return "north"; }
  if (direction == "e") { return "east"; }
  if (direction == "s") { return "south"; }
  if (direction == "w") { return "west"; }
  return direction;
}

string atlas_entrance(string path) {
  return entrances[path];
}

string starting_place() {
  return content["start"] ? content["start"] : "place/atlas/x0113/y3336";
}

int valid_place(string path) {
  return path && (place(path)
      || jvmud_invoke_lpc_object("system/atlas", "square_for_path", path));
}

string place_path(object room) {
  if (!room) { return ""; }
  if (jvmud_method_exists("atlas_path", room)) {
    return jvmud_invoke_lpc_object(room, "atlas_path");
  }
  return jvmud_lpc_object_id(room);
}

int relocate(object actor, string path) {
  mixed square;
  object room;

  square = jvmud_invoke_lpc_object("system/atlas", "square_for_path", path);
  if (square) {
    room = jvmud_invoke_lpc_object("system/atlas_places", "acquire", path, square);
    if (!room) {
      return 0;
    }
    jvmud_move_entity(actor, room);
  } else if (place(path) || path == "place/in-transit") {
    jvmud_move_entity(actor, path);
  } else {
    return 0;
  }
  return place_path(jvmud_entity_location(actor)) == path;
}

int room_number(string path) {
  mixed entry;
  mixed square;

  entry = place(path);
  if (entry) {
    return entry["id"];
  }
  square = jvmud_invoke_lpc_object("system/atlas", "square_for_path", path);
  return square ? 1 + square[1] * 5120 + square[0] : 0;
}

int wilderness(string path) {
  mixed entry;

  entry = place(path);
  if (entry) {
    return entry["environment"] == "wilderness";
  }
  return jvmud_invoke_lpc_object("system/atlas", "square_for_path", path) != 0;
}

void populate(object room, mixed *ids) {
  object entity;
  int index;

  for (index = 0; index < jvmud_size(ids); index += 1) {
    entity = jvmud_invoke_lpc_object("system/creatures", "create", ids[index]);
    if (!entity) {
      jvmud_raise_error("Unknown resident " + ids[index] + " in " + place_path(room) + ".");
    }
    jvmud_move_entity(entity, room);
  }
}
