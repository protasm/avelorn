nosave mapping data;
nosave string area_name;
nosave string area_id;
nosave string nation;
nosave int room_number;
nosave string plain_description;
nosave string *detail_labels;

void initialize(mixed first_load) {
  object room;
  mapping index;
  mapping area;
  string path;

  if (data) { return; }
  room = jvmud_current_lpc_object();
  path = jvmud_lpc_object_id(room);
  index = jvmud_invoke_lpc_object("system/world", "place", path);
  data = jvmud_invoke_lpc_object(room, "definition");
  if (!index || !jvmud_is_mapping(data) || index["environment"] != data["environment"]) {
    jvmud_raise_error("Invalid LPC Place definition for " + path + ".");
  }
  area_id = index["area"];
  room_number = index["id"];
  area = jvmud_invoke_lpc_object("system/world", "area", area_id);
  area_name = area["name"];
  nation = area["nation"];
  prepare_description(path);
  jvmud_invoke_lpc_object("system/world", "populate", room, data["npcs"]);
}

void prepare_description(string path) {
  string description;
  string word;
  string key;
  mixed details;
  mixed response;
  int opening;
  int start;
  int index;

  description = data["description"];
  details = data["details"];
  if (!jvmud_size(description) || (details && !jvmud_is_mapping(details))) {
    jvmud_raise_error("Invalid room description or details in " + path + ".");
  }
  if (!details) { details = ([ ]); }
  plain_description = "";
  detail_labels = ({ });
  opening = -1;
  for (index = 0; index < jvmud_size(description); index += 1) {
    if (description[index] == '[') {
      if (opening >= 0) {
        jvmud_raise_error("Nested detail marker in " + path + ".");
      }
      if (index > start) {
        plain_description += jvmud_extract_text(description, start, index - 1);
      }
      opening = index;
    } else if (description[index] == ']') {
      if (opening < 0 || opening + 1 == index) {
        jvmud_raise_error("Empty or unmatched detail marker in " + path + ".");
      }
      word = jvmud_extract_text(description, opening + 1, index - 1);
      key = jvmud_lowercase_text(word);
      response = details[key];
      if (!jvmud_is_string(response) || !jvmud_size(response)
          || word[0] <= ' ' || word[jvmud_size(word) - 1] <= ' '
          || jvmud_member(word, '\n') >= 0 || jvmud_member(detail_labels, key) >= 0) {
        jvmud_raise_error("Missing or invalid detail [" + word + "] in " + path + ".");
      }
      detail_labels += ({ key });
      plain_description += word;
      opening = -1;
      start = index + 1;
    }
  }
  if (opening >= 0 || jvmud_size(detail_labels) != jvmud_size(details)) {
    jvmud_raise_error("Unadvertised detail or unmatched bracket in " + path + ".");
  }
  if (start < jvmud_size(description)) {
    plain_description += jvmud_extract_text(description, start);
  }
}

void offer_interactions() {
  mixed *names;
  mixed *interactions;
  mapping action;
  int index;

  names = jvmud_mapping_keys(data["exits"]);
  for (index = 0; index < jvmud_size(names); index += 1) {
    jvmud_add_action("queue_exit", names[index]);
  }
  if (data["exits"]["north"]) { jvmud_add_action("queue_exit", "n"); }
  if (data["exits"]["east"]) { jvmud_add_action("queue_exit", "e"); }
  if (data["exits"]["south"]) { jvmud_add_action("queue_exit", "s"); }
  if (data["exits"]["west"]) { jvmud_add_action("queue_exit", "w"); }
  if (has_service("rest")) { jvmud_add_action("rest_here", "rest"); }
  if (has_service("training")) { jvmud_add_action("train_here", "train"); }
  if (has_service("market")) {
    jvmud_add_action("list_stock", "list");
    jvmud_add_action("buy", "buy");
    jvmud_add_action("sell", "sell");
  }
  jvmud_add_action("list_trails", "trails");
  if (jvmud_size(detail_labels)) {
    jvmud_add_action("list_details", "details");
  }
  if (jvmud_size(data["quests"])) {
    jvmud_add_action("noticeboard_work", "work");
    jvmud_add_action("noticeboard_report", "report");
    jvmud_add_action("read_notices", "notices");
  }
  action = data["action"];
  if (action) {
    jvmud_add_action("act_on_detail", action["verb"]);
  }
  interactions = data["actions"];
  for (index = 0; index < jvmud_size(interactions); index += 1) {
    jvmud_add_action("act_on_detail", interactions[index]["verb"]);
  }
}

int has_service(string name) {
  return jvmud_member(data["services"], name) >= 0;
}

int queue_exit(mixed ignored) {
  return jvmud_invoke_lpc_object("system/travel", "enqueue",
      jvmud_current_actor(),
      jvmud_invoke_lpc_object("system/world", "direction_name", jvmud_current_verb()), 1);
}

mixed query_travel_step(string direction) {
  string destination;
  int wilderness;
  int destination_wilderness;

  destination = data["exits"][direction];
  if (!destination) {
    return "There is no exit that way.";
  }
  destination_wilderness = jvmud_invoke_lpc_object("system/world", "wilderness", destination);
  wilderness = query_wilderness() || destination_wilderness;
  return ([
    "destination": destination,
    "cost": destination_wilderness ? 2 : 0,
    "wilderness": wilderness
  ]);
}

int query_wilderness() { return data["environment"] == "wilderness"; }
int query_safe() { return data["safe"]; }
string short() { return data["name"]; }
string query_area() { return area_name; }
string query_environment() { return data["environment"]; }
int query_gmcp_id() { return room_number; }

mapping query_gmcp_exits() {
  mapping exits;
  mapping destinations;
  mixed *names;
  string name;
  int index;

  exits = ([ ]);
  destinations = data["exits"];
  names = jvmud_mapping_keys(destinations);
  for (index = 0; index < jvmud_size(names); index += 1) {
    name = names[index];
    exits[name] = jvmud_invoke_lpc_object("system/world", "room_number", destinations[name]);
  }
  return exits;
}

void describe(object viewer) {
  write(short() + "\n" + (show_detail_hints(viewer) ? data["description"] : plain_description) + "\n");
  describe_details(viewer);
}

void describe_brief(object viewer) {
  write(short() + "\n");
  if (jvmud_size(detail_labels)) { write_details(show_detail_hints(viewer)); }
  describe_details(viewer);
}

int show_detail_hints(object viewer) {
  return viewer && jvmud_method_exists("query_inspect_hints", viewer)
      && jvmud_invoke_lpc_object(viewer, "query_inspect_hints");
}

void write_details(int mark) {
  string label;
  string line;
  int index;

  line = "";
  for (index = 0; index < jvmud_size(detail_labels); index += 1) {
    label = detail_labels[index];
    if (index) { line += ", "; }
    line += mark ? "[" + label + "]" : label;
  }
  write("Details: " + line + ".\n");
}

int list_details(mixed ignored) {
  write_details(show_detail_hints(jvmud_current_actor()));
  return 1;
}

int examine_detail(string target) {
  string label;

  label = jvmud_lowercase_text(target);
  if (jvmud_member(detail_labels, label) < 0) {
    return 0;
  }
  write(data["details"][label] + "\n");
  return 1;
}

void describe_details(object viewer) {
  object entity;
  mixed *exits;
  mixed *services;

  entity = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (entity) {
    if (entity != viewer && jvmud_method_exists("short", entity)) {
      write("  " + jvmud_invoke_lpc_object(entity, "short") + "\n");
    }
    entity = jvmud_next_entity_at(entity);
  }
  exits = jvmud_mapping_keys(data["exits"]);
  write("Exits: " + (jvmud_size(exits)
      ? jvmud_invoke_lpc_object("system/world", "join_names", exits, " ") : "none") + "\n");
  services = data["services"];
  if (jvmud_size(services)) {
    write("Services: " + jvmud_invoke_lpc_object("system/world", "join_names", services, ", ") + "\n");
  }
}

int act_on_detail(mixed argument) {
  mapping action;
  mixed *interactions;
  string verb;
  int index;

  verb = jvmud_current_verb();
  action = data["action"];
  if (action && matches_detail(action, verb, argument)) {
    return apply_detail(action);
  }
  interactions = data["actions"];
  for (index = 0; index < jvmud_size(interactions); index += 1) {
    action = interactions[index];
    if (matches_detail(action, verb, argument)) {
      return apply_detail(action);
    }
  }
  return 0;
}

int matches_detail(mapping action, string verb, mixed argument) {
  return action["verb"] == verb
      && (!jvmud_size(action["argument"]) || argument == action["argument"]);
}

int apply_detail(mapping action) {
  write(action["text"] + "\n");
  return jvmud_invoke_lpc_object(jvmud_current_actor(), "record_quest_action", action["tag"]);
}

int read_notices(mixed ignored) {
  mixed *assignments;
  string id;
  int index;

  assignments = data["quests"];
  write("The noticeboard carries these assignments:\n");
  for (index = 0; index < jvmud_size(assignments); index += 1) {
    id = assignments[index];
    write("  " + jvmud_invoke_lpc_object("system/quests", "title", id) + " [" + id + "]\n");
  }
  write("Use work <id> to accept or report <id> to turn one in.\n");
  return 1;
}

int noticeboard_work(mixed id) {
  if (!id) {
    return 0;
  }
  if (jvmud_member(data["quests"], id) < 0) {
    return 0;
  }
  return jvmud_invoke_lpc_object(jvmud_current_actor(), "offer_quest", id);
}

int noticeboard_report(mixed id) {
  if (!id) {
    return 0;
  }
  if (jvmud_member(data["quests"], id) < 0) {
    return 0;
  }
  return jvmud_invoke_lpc_object(jvmud_current_actor(), "turn_in_quest", id);
}

int rest_here(mixed ignored) {
  return jvmud_invoke_lpc_object(jvmud_current_actor(), "rest_at_shrine");
}

int train_here(mixed ignored) {
  return jvmud_invoke_lpc_object(jvmud_current_actor(), "complete_introductory_drill");
}

int list_stock(mixed ignored) {
  mixed *stock;
  mapping item;
  int index;

  stock = jvmud_invoke_lpc_object("system/items", "stock", area_id, nation);
  write("For sale:\n");
  for (index = 0; index < jvmud_size(stock); index += 1) {
    item = jvmud_invoke_lpc_object("system/items", "blueprint", stock[index]);
    write("  " + item["name"] + " (" + item["alias"] + ", "
        + jvmud_invoke_lpc_object("system/economy", "format_money", item["value"]) + ")\n");
  }
  write("Use buy <item> or sell <item>.\n");
  return 1;
}

int buy(mixed requested) {
  mixed *stock;
  mapping item;
  string name;
  int index;

  if (!requested) { return list_stock(0); }
  name = jvmud_lowercase_text(requested);
  if (name == "ration") { name = "bread"; }
  stock = jvmud_invoke_lpc_object("system/items", "stock", area_id, nation);
  for (index = 0; index < jvmud_size(stock); index += 1) {
    item = jvmud_invoke_lpc_object("system/items", "blueprint", stock[index]);
    if (name == item["alias"] || name == item["name"] || name == stock[index]) {
      return jvmud_invoke_lpc_object(jvmud_current_actor(), "purchase_blueprint", stock[index]);
    }
  }
  write("Ask to list the wares sold here.\n");
  return 1;
}

int sell(mixed item) {
  return jvmud_invoke_lpc_object(jvmud_current_actor(), "sell_item", item);
}

int list_trails(mixed ignored) {
  mapping trails;
  mapping trail;
  mapping destination;
  mixed *keys;
  int index;
  int found;

  trails = jvmud_invoke_lpc_object("system/world", "section", "trails");
  keys = jvmud_mapping_keys(trails);
  for (index = 0; index < jvmud_size(keys); index += 1) {
    trail = trails[keys[index]];
    if (trail["from"] == area_id) {
      destination = jvmud_invoke_lpc_object("system/world", "area", trail["to"]);
      write(destination["name"] + " — " + trail["description"] + "\n");
      found = 1;
    }
  }
  if (!found) {
    write("There are no marked trails from here.\n");
  }
  return 1;
}
