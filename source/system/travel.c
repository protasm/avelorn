// Both movement and camping use world ticks, so neither can bank time while idle.
nosave mapping travelers;

void initialize(mixed first_load) {
  travelers = ([ ]);
}

mapping travel_state(object actor) {
  mapping state;

  state = travelers[actor];
  if (!state) {
    state = ([
        "steps": ({ }),
        "overflow": 0,
        "cooldown": 0,
        "camp_place": 0,
        "camp_ticks": 0
    ]);
    travelers[actor] = state;
  }
  return state;
}

void end(object actor) {
  jvmud_mapping_delete(travelers, actor);
}

void clear(object actor) {
  mapping state;

  state = travelers[actor];
  if (state) {
    state["steps"] = ({ });
    state["overflow"] = 0;
    state["camp_place"] = 0;
    state["camp_ticks"] = 0;
  }
}

int count(object actor) {
  mapping state;

  state = travelers[actor];
  if (state) {
    return jvmud_size(state["steps"]);
  }
  return 0;
}

string current_path(object actor) {
  return jvmud_invoke_lpc_object("system/world", "place_path", jvmud_entity_location(actor));
}

void clear_follow(object actor) {
  mapping state;
  mixed *steps;
  mixed *manual_steps;
  int index;

  state = travelers[actor];
  if (!state) { return; }
  steps = state["steps"];
  manual_steps = ({ });
  for (index = 0; index < jvmud_size(steps); index++) {
    if (!jvmud_is_mapping(steps[index])) {
      manual_steps += ({ steps[index] });
    }
  }
  state["steps"] = manual_steps;
}

int enqueue_follow(object actor, string origin, string direction, string destination) {
  mapping state;
  mixed *steps;
  mixed last;
  int count;

  if (!jvmud_interactive(actor) || jvmud_invoke_lpc_object(actor, "query_travel_blocked")) {
    return 0;
  }
  state = travel_state(actor);
  steps = state["steps"];
  count = jvmud_size(steps);
  if (state["camp_ticks"] || count >= 8) { return 0; }
  if (!count && current_path(actor) != origin) { return 0; }
  if (count) {
    last = steps[count - 1];
    if (!jvmud_is_mapping(last) || last["destination"] != origin) {
      return 0;
    }
  }
  state["steps"] = steps + ({ ([
      "origin": origin,
      "direction": direction,
      "destination": destination
  ]) });
  return 1;
}

int enqueue(object actor, string direction, int requested) {
  mapping state;
  int index;

  if (!jvmud_interactive(actor)) {
    return 0;
  }
  if (jvmud_invoke_lpc_object(actor, "query_travel_blocked")) {
    jvmud_write_to_lpc_object(actor, "You cannot walk right now.\n");
    return 0;
  }
  if (!jvmud_size(direction) || requested < 1 || requested > 8) {
    jvmud_write_to_lpc_object(actor, "Choose a direction and up to eight steps.\n");
    return 0;
  }
  jvmud_invoke_lpc_object("system/party", "stop_following", actor, "");
  state = travel_state(actor);
  if (jvmud_size(state["steps"]) + requested > 8) {
    if (!state["overflow"]) {
      state["overflow"] = 1;
      jvmud_write_to_lpc_object(actor, "Movement queue full. Type stop to clear it.\n");
    }
    return 0;
  }
  if (state["camp_ticks"]) {
    clear(actor);
    jvmud_write_to_lpc_object(actor, "You break camp and prepare to move.\n");
  }
  for (index = 0; index < requested; index++) {
    state["steps"] = state["steps"] + ({ direction });
  }
  return 1;
}

int camp(object actor) {
  object place;
  mapping state;

  if (!jvmud_interactive(actor)) {
    return 0;
  }
  place = jvmud_entity_location(actor);
  if (jvmud_invoke_lpc_object(actor, "query_travel_blocked")) {
    jvmud_write_to_lpc_object(actor, "You cannot make camp right now.\n");
    return 0;
  }
  if (!place || !jvmud_method_exists("query_wilderness", place)
      || !jvmud_invoke_lpc_object(place, "query_wilderness")) {
    jvmud_write_to_lpc_object(actor, "Make camp in the wilderness. Seek shelter here.\n");
    return 0;
  }
  if (jvmud_invoke_lpc_object(actor, "query_travel_reserve") >= 100) {
    jvmud_write_to_lpc_object(actor, "Your travel reserve is already full.\n");
    return 0;
  }
  if (jvmud_method_exists("query_has_travel_ration", actor)
      && !jvmud_invoke_lpc_object(actor, "query_has_travel_ration")) {
    jvmud_write_to_lpc_object(actor, "You need a ration to make camp.\n");
    return 0;
  }
  state = travel_state(actor);
  if (state["camp_ticks"]) {
    jvmud_write_to_lpc_object(actor, "You are already making camp.\n");
    return 0;
  }
  jvmud_invoke_lpc_object("system/party", "stop_following", actor, "");
  clear(actor);
  state["camp_place"] = place;
  // The travel tick runs after the command tick that starts camp.
  state["camp_ticks"] = 16;
  jvmud_write_to_lpc_object(actor, "You make camp. Rest here for fifteen seconds.\n");
  return 1;
}

string status(object actor) {
  mapping state;
  string result;

  state = travelers[actor];
  result = "Travel reserve: "
      + jvmud_invoke_lpc_object(actor, "query_travel_reserve") + "/100."
      + " Movement queue: " + count(actor) + "/8.";
  if (state && state["camp_ticks"]) {
    result += " You are making camp.";
  }
  return result;
}

int drain(object actor) {
  mapping state;
  mapping step;
  mixed *steps;
  mixed edge;
  mixed intent;
  object place;
  string origin;
  string direction;
  int moved;
  int wilderness;
  int reserve;
  int remaining;
  int cost;
  int success;

  state = travelers[actor];
  if (!state || !jvmud_interactive(actor)) {
    return 0;
  }
  if (state["cooldown"] > 0) {
    state["cooldown"] -= 1;
  }
  if (state["camp_ticks"]) {
    return advance_camp(actor, state);
  }
  for (moved = 0; moved < 4 && jvmud_size(state["steps"]) > 0; moved++) {
    if (jvmud_invoke_lpc_object(actor, "query_travel_blocked")) {
      jvmud_invoke_lpc_object("system/party", "stop_following", actor, "");
      clear(actor);
      return moved > 0;
    }
    steps = state["steps"];
    intent = steps[0];
    place = jvmud_entity_location(actor);
    origin = current_path(actor);
    direction = jvmud_is_mapping(intent) ? intent["direction"] : intent;
    if (jvmud_is_mapping(intent) && intent["origin"] != origin) {
      return lose_trail(actor);
    }
    edge = 0;
    if (place && jvmud_method_exists("query_travel_step", place)) {
      if (catch(edge = jvmud_invoke_lpc_object(place, "query_travel_step", direction))) {
        edge = 0;
      }
    }
    if (jvmud_is_mapping(intent)
        && (!jvmud_is_mapping(edge) || intent["destination"] != edge["destination"])) {
      return lose_trail(actor);
    }
    if (!jvmud_is_mapping(edge)) {
      clear(actor);
      if (jvmud_is_string(edge)) {
        jvmud_write_to_lpc_object(actor, edge + "\n");
      } else {
        jvmud_write_to_lpc_object(actor, "You cannot go " + direction + " from here.\n");
      }
      return 1;
    }

    step = edge;
    cost = step["cost"];
    wilderness = step["wilderness"] || cost > 0;
    if (wilderness && (moved > 0 || state["cooldown"] > 0)) {
      return moved > 0;
    }
    if (jvmud_size(steps) > 1) {
      state["steps"] = steps[1..];
    } else {
      state["steps"] = ({ });
    }
    state["overflow"] = 0;
    success = 0;
    if (catch(success = jvmud_invoke_lpc_object(actor, "travel_to", direction, step["destination"]))
        || !success) {
      if (jvmud_is_mapping(intent)) { return lose_trail(actor); }
      clear(actor);
      jvmud_write_to_lpc_object(actor, "You cannot complete that move.\n");
      return 1;
    }
    if (cost > 0) {
      reserve = jvmud_invoke_lpc_object(actor, "query_travel_reserve");
      if (reserve > 0) {
        remaining = jvmud_invoke_lpc_object(actor, "adjust_travel_reserve", -cost);
        if (remaining <= 0) {
          jvmud_write_to_lpc_object(actor, "You are exhausted. Wilderness travel will be slower.\n");
        } else if (reserve > 25 && remaining <= 25) {
          jvmud_write_to_lpc_object(actor, "Your travel reserve is low. Consider making camp.\n");
        }
        reserve = remaining;
      }
      if (reserve <= 0) {
        state["cooldown"] = 3;
      }
    }
    jvmud_invoke_lpc_object("system/party", "schedule_followers", actor,
        origin, direction, step["destination"]);
    if (wilderness) {
      return 1;
    }
  }
  return moved > 0;
}

int lose_trail(object actor) {
  jvmud_invoke_lpc_object("system/party", "stop_following", actor,
      "You lose your party leader's trail. Regroup before following again.");
  return 1;
}

int advance_camp(object actor, mapping state) {
  if (jvmud_entity_location(actor) != state["camp_place"]
      || jvmud_invoke_lpc_object(actor, "query_travel_blocked")) {
    clear(actor);
    jvmud_write_to_lpc_object(actor, "Your rest is interrupted.\n");
    return 1;
  }
  state["camp_ticks"] -= 1;
  if (state["camp_ticks"] > 0) {
    return 0;
  }
  state["camp_place"] = 0;
  if (catch(jvmud_invoke_lpc_object(actor, "complete_travel_camp"))) {
    jvmud_write_to_lpc_object(actor, "You could not finish making camp.\n");
  }
  return 1;
}
