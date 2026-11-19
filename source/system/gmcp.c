void initialize(mixed first_load) {
}

int room_id(object place) {
  if (place && jvmud_method_exists("query_gmcp_id", place)) {
    return jvmud_invoke_lpc_object(place, "query_gmcp_id");
  }
  return 0;
}

void send_room(object player) {
  object place;
  mapping exits;
  mapping info;
  int number;

  if (!jvmud_gmcp_enabled(player)) { return; }
  place = jvmud_entity_location(player);
  if (!place) { return; }
  number = room_id(place);
  if (number <= 0) { return; }
  if (jvmud_method_exists("query_gmcp_exits", place)) {
    exits = jvmud_invoke_lpc_object(place, "query_gmcp_exits");
  } else {
    exits = ([ ]);
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
