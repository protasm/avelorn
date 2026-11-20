string terminal(object actor) {
  object location;

  location = jvmud_entity_location(actor);
  return jvmud_invoke_lpc_object("system/world", "place_path", location);
}

string destination_name(string path) {
  mixed place;
  mixed area;

  place = jvmud_invoke_lpc_object("system/world", "place", path);
  if (place) {
    area = jvmud_invoke_lpc_object("system/world", "area", place["area"]);
    if (area && area["name"]) {
      return area["name"];
    }
    return place["name"];
  }
  return "a waystation";
}

string travel_time(int seconds) {
  int minutes;

  minutes = seconds / 60;
  seconds = seconds % 60;
  if (!minutes) {
    return seconds + " sec";
  }
  if (!seconds) {
    return minutes + " min";
  }
  return minutes + " min " + seconds + " sec";
}

string status(object actor) {
  mixed trip;
  mixed route;
  int remaining;
  string name;

  trip = jvmud_invoke_lpc_object(actor, "query_transit");
  if (!trip) {
    return "";
  }
  route = jvmud_invoke_lpc_object("system/world", "section", "routes")[trip["id"]];
  name = route ? route["name"] : "Your passage";
  remaining = trip["arrives_at"] - jvmud_time();
  if (remaining <= 0) {
    return name + " is due to arrive at " + destination_name(trip["destination"]) + ".";
  }
  return name + " is bound for " + destination_name(trip["destination"])
      + ". Arrival in " + travel_time(remaining) + ".";
}

int list_routes(object actor) {
  mapping routes;
  mixed *ids;
  mapping route;
  string current_terminal;
  string listing;
  string fare;
  string travel_status;
  int index;

  travel_status = status(actor);
  if (jvmud_size(travel_status)) {
    avelorn_write_to(actor, travel_status + "\n");
    return 1;
  }
  current_terminal = terminal(actor);
  routes = jvmud_invoke_lpc_object("system/world", "section", "routes");
  ids = jvmud_mapping_keys(routes);
  listing = "";
  for (index = 0; index < jvmud_size(ids); index += 1) {
    route = routes[ids[index]];
    if (route["from"] == current_terminal) {
      fare = route["fare"] ? route["fare"] + " copper" : "free";
      listing += "  " + route["name"] + " to " + destination_name(route["to"])
          + " (" + travel_time(route["duration"]) + ", " + fare
          + ") [" + ids[index] + "]\n";
    }
  }
  if (jvmud_size(listing)) {
    avelorn_write_to(actor, "Departures:\n" + listing
        + "Use board <destination> or board <route id>.\n");
  } else {
    avelorn_write_to(actor, "There are no departures from here.\n");
  }
  return 1;
}

string route_from(mapping routes, string current_terminal, string name) {
  mixed *ids;
  mapping route;
  string destination;
  string match;
  int index;

  if (routes[name] && routes[name]["from"] == current_terminal) {
    return name;
  }
  ids = jvmud_mapping_keys(routes);
  for (index = 0; index < jvmud_size(ids); index += 1) {
    route = routes[ids[index]];
    if (route["from"] == current_terminal) {
      destination = jvmud_lowercase_text(destination_name(route["to"]));
      if (name == destination || name == jvmud_lowercase_text(route["name"])) {
        if (match) { return ""; }
        match = ids[index];
      }
    }
  }
  return match ? match : "";
}

int board(object actor, mixed request) {
  mapping routes;
  mapping route;
  string name;
  string id;
  string description;

  name = request ? jvmud_lowercase_text(jvmud_invoke_lpc_object(
      "system/input", "trim_line", jvmud_to_string(request))) : "";
  if (!jvmud_size(name) || jvmud_invoke_lpc_object(actor, "query_transit")) {
    return list_routes(actor);
  }
  if (jvmud_invoke_lpc_object(actor, "query_travel_blocked")) {
    avelorn_write_to(actor, "You cannot board right now.\n");
    return 1;
  }

  routes = jvmud_invoke_lpc_object("system/world", "section", "routes");
  id = route_from(routes, terminal(actor), name);
  if (!jvmud_size(id)) {
    avelorn_write_to(actor, "No single departure matches. Use routes to see your options.\n");
    return 1;
  }
  route = routes[id];
  if (route["duration"] <= 0 || route["fare"] < 0
      || !jvmud_invoke_lpc_object("system/world", "valid_place", route["to"])) {
    avelorn_write_to(actor, "That passage is temporarily unavailable.\n");
    return 1;
  }
  if (!jvmud_invoke_lpc_object(actor, "begin_transit", id, route["to"],
      jvmud_time() + route["duration"], route["fare"])) {
    return 1;
  }

  jvmud_invoke_lpc_object("system/party", "stop_followers", actor);
  jvmud_invoke_lpc_object("system/party", "stop_following", actor, "");
  jvmud_invoke_lpc_object("system/input", "clear", actor);
  jvmud_invoke_lpc_object("system/travel", "clear", actor);
  avelorn_write_to(actor, "You board " + route["name"] + " for "
      + destination_name(route["to"]) + ".\n");
  description = route["description"];
  if (jvmud_size(description)) {
    avelorn_write_to(actor, description + "\n");
  }
  jvmud_dispatch_entity_command(actor, "look");
  return 1;
}

int tick(object actor) {
  mixed trip;
  string destination;

  trip = jvmud_invoke_lpc_object(actor, "query_transit");
  if (!trip || jvmud_time() < trip["arrives_at"]) {
    return 0;
  }
  destination = trip["destination"];
  if (!jvmud_invoke_lpc_object("system/world", "valid_place", destination)) {
    destination = jvmud_invoke_lpc_object("system/world", "starting_place");
  }
  if (!jvmud_invoke_lpc_object(actor, "finish_transit", trip["id"], destination)) {
    return 0;
  }
  jvmud_invoke_lpc_object("system/input", "clear", actor);
  jvmud_invoke_lpc_object("system/travel", "clear", actor);
  avelorn_write_to(actor, "You arrive at " + destination_name(destination) + ".\n");
  jvmud_dispatch_entity_command(actor, "look");
  return 1;
}
