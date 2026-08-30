mapping learned_exits;

void initialize(mixed first_load) {
  learned_exits = ([ ]);
}

int room_id(object place) {
  if (place && jvmud_method_exists("query_gmcp_id", place)) {
    return jvmud_invoke_lpc_object(place, "query_gmcp_id");
  }
  return 0;
}

void record_travel(object origin, string direction, string destination) {
  string origin_path;
  mapping exits;

  if (!origin || !direction || !destination) { return; }
  origin_path = jvmud_lpc_object_id(origin);
  exits = learned_exits[origin_path];
  if (!exits) {
    exits = ([ ]);
    learned_exits[origin_path] = exits;
  }
  exits[direction] = destination;
}

void send_room(object player) {
  object place;
  string path;
  mapping exits;
  mapping info;
  int number;

  if (!jvmud_gmcp_enabled(player)) { return; }
  place = jvmud_entity_location(player);
  if (!place) { return; }
  number = room_id(place);
  if (number <= 0) { return; }
  path = jvmud_lpc_object_id(place);
  if (jvmud_method_exists("query_gmcp_exits", place)) {
    exits = jvmud_invoke_lpc_object(place, "query_gmcp_exits");
  } else {
    exits = learned_exits[path];
    if (!exits) { exits = ([ ]); }
  }
  info = ([
    "num": number,
    "name": jvmud_invoke_lpc_object(place, "short"),
    "area": area_name(place),
    "environment": environment_name(place),
    "details": ({ }),
    "exits": exits
  ]);
  jvmud_send_gmcp("Room.Info", info);
}

string area_name(object place) {
  if (jvmud_method_exists("query_area", place)) {
    return jvmud_invoke_lpc_object(place, "query_area");
  }
  return "Avelorn";
}

string environment_name(object place) {
  if (jvmud_method_exists("query_environment", place)) {
    return jvmud_invoke_lpc_object(place, "query_environment");
  }
  return "unknown";
}
