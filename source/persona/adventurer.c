string account_id;
string password_hash;
nosave string pending_password;
nosave int entered_world;
nosave int travel_dirty;
nosave int ticks_since_save;
nosave int combat_acted;
nosave int guard_power;
nosave int guard_ticks;
nosave int vitals_changed;
string character_name;
string gender;
string character_race;
string character_class;
string guild_id;
string combat_style;
int account_created;
int save_version;
int snapshot_generation;
int level;
int experience;
int strength;
int dexterity;
int constitution;
int intelligence;
int wisdom;
int charisma;
int max_health;
int health;
int max_resource;
int resource;
int copper;
int attribute_points;
int introductory_drill_completed;
string inventory_state;
nosave int inventory_materialized;
string quest_state;
nosave mapping quest_stages;
nosave mapping quest_counts;
nosave mapping quest_flags;
nosave int quests_materialized;
nosave object combat_target;
int brief_mode;
int inspect_hints;
string saved_place;
string sanctuary;
int travel_reserve = 100;
string transit_id;
string transit_destination;
int transit_arrives_at;

void initialize(mixed first_load) {
  if (!account_id) {
    account_id = "";
  }
  if (!password_hash) {
    password_hash = "";
  }
  if (!pending_password) {
    pending_password = "";
  }
  if (!character_name) {
    character_name = "unformed adventurer";
  }
  if (!gender) {
    gender = "non-binary";
  }
  if (!character_class) {
    character_class = "";
  }
  if (!save_version) {
    save_version = 1;
  }
  if (!level) {
    level = 1;
  }
  if (!inventory_state) {
    inventory_state = "";
  }
  if (!quest_state) {
    quest_state = "";
  }
  if (!saved_place) {
    saved_place = "";
  }
  if (!account_created && !copper) {
    copper = 120;
  }
}

void begin_session() {
  write("\nWelcome to Avelorn.\n");
  avelorn_prompt("Account ID: ");
  jvmud_capture_session_input("receive_account_id", 0);
}

void receive_account_id(mixed value) {
  string requested;
  int restored;

  requested = normalize_account_id(value);
  if (!valid_account_id(requested)) {
    write("Use 3-24 letters, numbers, underscores, or dashes.\n");
    avelorn_prompt("Account ID: ");
    jvmud_capture_session_input("receive_account_id", 0);
    return;
  }

  account_id = requested;
  restored = restore_account(requested);
  if (restored == 1) {
    avelorn_prompt("Password: ");
    jvmud_capture_session_input("receive_login_password", 1);
    return;
  }

  if (restored < 0) {
    write("This account could not be read. Please contact an operator.\n");
    prompt_for_account();
    return;
  }
  avelorn_prompt("No account named " + requested + " exists. Create it? (yes/no) ");
  jvmud_capture_session_input("confirm_account_creation", 0);
}

void prompt_for_account() {
  account_id = "";
  pending_password = "";
  avelorn_prompt("Account ID: ");
  jvmud_capture_session_input("receive_account_id", 0);
}

int account_exists(string stem) {
  return jvmud_invoke_lpc_object("system/accounts", "exists", stem)
      || jvmud_invoke_lpc_object("system/accounts", "exists", stem + ".0")
      || jvmud_invoke_lpc_object("system/accounts", "exists", stem + ".1");
}

int restore_account(string requested) {
  string stem;
  string candidate;
  int even_generation;
  int odd_generation;

  stem = "accounts/" + requested;
  even_generation = jvmud_invoke_lpc_object(
      "system/accounts", "checkpoint_generation", stem + ".0", requested);
  odd_generation = jvmud_invoke_lpc_object(
      "system/accounts", "checkpoint_generation", stem + ".1", requested);
  if (even_generation || odd_generation) {
    candidate = even_generation > odd_generation ? stem + ".0" : stem + ".1";
  } else {
    candidate = stem;
  }
  account_created = 0;
  password_hash = "";
  character_race = jvmud_invoke_lpc_object("system/races", "default_race");
  snapshot_generation = 0;
  if (jvmud_restore_lpc_object_state(candidate) && account_created
      && account_id == requested && jvmud_size(password_hash) && valid_class(character_class)) {
    if (!jvmud_size(character_race)) {
      character_race = jvmud_invoke_lpc_object("system/races", "default_race");
    }
    return jvmud_invoke_lpc_object("system/races", "known", character_race) ? 1 : -1;
  }
  return account_exists(stem) ? -1 : 0;
}

int claim_account() {
  if (jvmud_invoke_lpc_object(
      "system/accounts", "claim", account_id, jvmud_current_lpc_object())) {
    return 1;
  }
  write("That account is already playing.\n");
  prompt_for_account();
  return 0;
}

void confirm_account_creation(mixed value) {
  string answer;

  answer = jvmud_lowercase_text(value);
  if (answer == "yes" || answer == "y") {
    avelorn_prompt("Choose a password: ");
    jvmud_capture_session_input("receive_new_password", 1);
    return;
  }
  if (answer == "no" || answer == "n") {
    account_id = "";
    write("No account was created.\n");
    avelorn_prompt("Account ID: ");
    jvmud_capture_session_input("receive_account_id", 0);
    return;
  }
  avelorn_prompt("Please answer yes or no: ");
  jvmud_capture_session_input("confirm_account_creation", 0);
}

void receive_new_password(mixed value) {
  mixed problem;

  problem = password_problem(value);
  if (problem) {
    write(problem + "\n");
    avelorn_prompt("Choose a password: ");
    jvmud_capture_session_input("receive_new_password", 1);
    return;
  }

  pending_password = value;
  avelorn_prompt("Password again: ");
  jvmud_capture_session_input("confirm_new_password", 1);
}

void confirm_new_password(mixed value) {
  if (value != pending_password) {
    pending_password = "";
    write("Those passwords did not match.\n");
    avelorn_prompt("Choose a password: ");
    jvmud_capture_session_input("receive_new_password", 1);
    return;
  }

  password_hash = jvmud_hash_password(value);
  pending_password = "";
  avelorn_prompt("Character name: ");
  jvmud_capture_session_input("receive_character_name", 0);
}

void receive_login_password(mixed value) {
  string requested;

  if (!jvmud_verify_password(value, password_hash)) {
    write("That password did not match.\n");
    avelorn_prompt("Password: ");
    jvmud_capture_session_input("receive_login_password", 1);
    return;
  }

  requested = account_id;
  if (!claim_account()) {
    return;
  }
  // The prior session may have written a newer checkpoint after the password prompt.
  if (restore_account(requested) != 1 || !jvmud_verify_password(value, password_hash)) {
    jvmud_invoke_lpc_object("system/accounts", "release", requested, jvmud_current_lpc_object());
    write("The account changed. Sign in again.\n");
    prompt_for_account();
    return;
  }
  pending_password = "";
  if (save_version < 2) {
    quest_state = "";
    save_version = 2;
  }
  if (!valid_gender(gender)) {
    avelorn_prompt("Choose your character's gender (male/female): ");
    jvmud_capture_session_input("receive_returning_gender", 0);
    return;
  }
  enter_avelorn(1);
}

void receive_returning_gender(mixed value) {
  string selected;

  selected = jvmud_lowercase_text(value);
  if (!valid_gender(selected)) {
    avelorn_prompt("Please choose male or female: ");
    jvmud_capture_session_input("receive_returning_gender", 0);
    return;
  }
  gender = selected;
  enter_avelorn(1);
}

void receive_character_name(mixed value) {
  if (!valid_character_name(value)) {
    write("Use 2-24 letters, spaces, apostrophes, or dashes.\n");
    avelorn_prompt("Character name: ");
    jvmud_capture_session_input("receive_character_name", 0);
    return;
  }

  character_name = jvmud_capitalize_text(jvmud_lowercase_text(value));
  avelorn_prompt("Gender (male/female): ");
  jvmud_capture_session_input("receive_gender", 0);
}

void receive_gender(mixed value) {
  mixed *races;
  string selected;

  selected = jvmud_lowercase_text(value);
  if (!valid_gender(selected)) {
    avelorn_prompt("Please choose male or female: ");
    jvmud_capture_session_input("receive_gender", 0);
    return;
  }

  gender = selected;
  races = jvmud_invoke_lpc_object("system/races", "ids");
  if (jvmud_size(races) > 1) {
    prompt_for_race();
    return;
  }
  character_race = jvmud_invoke_lpc_object("system/races", "default_race");
  prompt_for_class();
}

void prompt_for_race() {
  mixed *races;

  races = jvmud_invoke_lpc_object("system/races", "ids");
  avelorn_prompt("Race (" + jvmud_invoke_lpc_object("system/world", "join_names", races, "/") + "): ");
  jvmud_capture_session_input("receive_race", 0);
}

void receive_race(mixed value) {
  string selected;

  selected = jvmud_lowercase_text(value);
  if (!jvmud_invoke_lpc_object("system/races", "known", selected)) {
    prompt_for_race();
    return;
  }
  character_race = selected;
  prompt_for_class();
}

void prompt_for_class() {
  mixed *classes;

  classes = jvmud_invoke_lpc_object("system/combat_rules", "classes");
  avelorn_prompt("Class (" + jvmud_invoke_lpc_object("system/world", "join_names", classes, "/") + "): ");
  jvmud_capture_session_input("receive_class", 0);
}

void receive_class(mixed value) {
  string selected;

  selected = jvmud_lowercase_text(value);
  if (!valid_class(selected)) {
    prompt_for_class();
    return;
  }

  if (account_exists("accounts/" + account_id)) {
    write("That account has already been created. Sign in to play it.\n");
    prompt_for_account();
    return;
  }
  if (!claim_account()) {
    return;
  }
  character_class = selected;
  guild_id = "";
  combat_style = "";
  level = 1;
  experience = 0;
  copper = 120;
  attribute_points = 0;
  introductory_drill_completed = 0;
  brief_mode = 0;
  inspect_hints = 0;
  saved_place = "";
  sanctuary = "";
  travel_reserve = 100;
  transit_id = "";
  transit_destination = "";
  transit_arrives_at = 0;
  configure_starting_attributes();
  account_created = 1;
  save_version = 2;
  inventory_materialized = 1;
  quests_materialized = 1;
  quest_stages = ([ ]);
  quest_counts = ([ ]);
  quest_flags = ([ ]);
  grant_starter_kit();
  recalculate_resources(1);
  enter_avelorn(0);
}

void enter_avelorn(int returning) {
  entered_world = 1;
  jvmud_enable_commands();
  materialize_inventory();
  materialize_quests();
  recalculate_resources(0);
  jvmud_bind_entity_alias(
      jvmud_current_lpc_object(),
      "entity",
      jvmud_lowercase_text(character_name));
  jvmud_bind_entity_alias(
      jvmud_current_lpc_object(),
      "living",
      jvmud_lowercase_text(character_name));

  if (!returning || !jvmud_invoke_lpc_object("system/world", "valid_place", saved_place)) {
    saved_place = starting_place();
  }
  if (!jvmud_invoke_lpc_object("system/world", "relocate", jvmud_current_lpc_object(),
      jvmud_size(transit_id) ? "place/in-transit" : saved_place)) {
    jvmud_invoke_lpc_object("system/world", "relocate", jvmud_current_lpc_object(), starting_place());
  }

  jvmud_schedule_deferred_callback("scheduled_update", 1);
  save_character();
  if (returning) {
    write("\nWelcome back, " + character_name + ".\n\n");
  } else {
    write("Welcome to Avelorn, " + character_name + ".\n\n");
  }
  look(0);
  send_gmcp_character();
  if (!returning) {
    write("Type work to ask for an assignment, or help for commands.\n");
  }
  jvmud_invoke_lpc_object("system/input", "begin", jvmud_current_lpc_object());
}

void client_protocol_changed(string protocol, int enabled) {
  if (enabled && jvmud_lowercase_text(protocol) == "gmcp" && entered_world) {
    send_gmcp_character();
    jvmud_invoke_lpc_object("system/gmcp", "send_room", jvmud_current_lpc_object());
  }
}

void receive_gmcp(string package_name, mixed payload) {
  string normalized;

  normalized = jvmud_lowercase_text(package_name);
  if (normalized == "core.ping") {
    jvmud_send_gmcp("Core.Ping");
    return;
  }
  if (entered_world && (normalized == "core.supports.set"
      || normalized == "core.supports.add")) {
    send_gmcp_character();
    jvmud_invoke_lpc_object("system/gmcp", "send_room", jvmud_current_lpc_object());
  }
}

void send_gmcp_character() {
  mapping vitals;

  if (!entered_world || !jvmud_gmcp_enabled()) {
    return;
  }
  jvmud_send_gmcp("Char.Name", ([
    "name": character_name,
    "fullname": character_name
  ]));
  jvmud_send_gmcp("Char.Base", ([
    "name": character_name,
    "class": character_class,
    "race": character_race
  ]));
  vitals = ([
    "hp": health,
    "maxhp": max_health,
    "health": health,
    "maxHealth": max_health,
    "mana": resource,
    "maxmana": max_resource,
    "moves": resource,
    "maxmoves": max_resource,
    "resource": resource,
    "maxresource": max_resource
  ]);
  jvmud_send_gmcp("Char.Vitals", vitals);
  jvmud_send_gmcp("Char.Maxstats", ([
    "maxhp": max_health,
    "maxmana": max_resource,
    "maxmoves": max_resource
  ]));
  jvmud_send_gmcp("Char.StatusVars", ([
    "level": "Level",
    "class": "Class",
    "experience": "Experience",
    "resource": resource_name()
  ]));
  jvmud_send_gmcp("Char.Status", ([
    "level": level,
    "class": character_class,
    "experience": experience,
    "tnl": level < level_cap() ? experience_for_next_level() - experience : 0,
    "resource": resource
  ]));
}

void end_session() {
  combat_target = 0;
  jvmud_cancel_deferred_callback("scheduled_update");
  jvmud_invoke_lpc_object("system/input", "end", jvmud_current_lpc_object());
  jvmud_invoke_lpc_object("system/travel", "end", jvmud_current_lpc_object());
  if (entered_world) {
    save_character();
    entered_world = 0;
    jvmud_invoke_lpc_object("system/party", "detach", jvmud_current_lpc_object());
    clear_materialized_inventory();
  }
  if (account_id) {
    jvmud_invoke_lpc_object(
        "system/accounts", "release", account_id, jvmud_current_lpc_object());
  }
}

int save_character() {
  string stem;
  object place;

  if (entered_world && account_created && jvmud_size(account_id) > 0
      && jvmud_size(password_hash) > 0) {
    pending_password = "";
    place = jvmud_entity_location(jvmud_current_lpc_object());
    if (place) {
      saved_place = jvmud_invoke_lpc_object("system/world", "place_path", place);
    }
    if (inventory_materialized) {
      snapshot_inventory();
    }
    if (quests_materialized) {
      snapshot_quests();
    }
    snapshot_generation += 1;
    stem = "accounts/" + account_id + "." + (snapshot_generation % 2);
    if (!jvmud_save_lpc_object_state(stem)) {
      snapshot_generation -= 1;
      write("Warning: Avelorn could not write your character snapshot to the host filesystem.\n");
      return 0;
    }
    travel_dirty = 0;
    ticks_since_save = 0;
    return 1;
  }
  return 0;
}

void offer_interactions() {
  jvmud_add_action("look", "look");
  jvmud_add_action("look", "l");
  jvmud_add_action("look", "examine");
  jvmud_add_action("look", "exa");
  jvmud_add_action("brief", "brief");
  jvmud_add_action("inspection_hints", "hints");
  jvmud_add_action("score", "score");
  jvmud_add_action("inventory", "inventory");
  jvmud_add_action("inventory", "i");
  jvmud_add_action("equipment", "equipment");
  jvmud_add_action("equipment", "eq");
  jvmud_add_action("get_item", "get");
  jvmud_add_action("get_item", "take");
  jvmud_add_action("drop_item", "drop");
  jvmud_add_action("equip_item", "equip");
  jvmud_add_action("unequip_item", "unequip");
  jvmud_add_action("use_item", "use");
  jvmud_add_action("attack", "attack");
  jvmud_add_action("attack", "fight");
  jvmud_add_action("consider", "consider");
  jvmud_add_action("class_ability", "ability");
  jvmud_add_action("class_ability", "technique");
  jvmud_add_action("list_techniques", "abilities");
  jvmud_add_action("select_style", "style");
  jvmud_add_action("brace", "brace");
  jvmud_add_action("quests", "quests");
  jvmud_add_action("quests", "journal");
  jvmud_add_action("money", "money");
  jvmud_add_action("improve", "improve");
  jvmud_add_action("pronouns_command", "pronouns");
  jvmud_add_action("who", "who");
  jvmud_add_action("say", "say");
  jvmud_add_action("tell", "tell");
  jvmud_add_action("help", "help");
  jvmud_add_action("save_command", "save");
  jvmud_add_action("quit", "quit");
  jvmud_add_action("walk_route", "walk");
  jvmud_add_action("flee", "flee");
  jvmud_add_action("camp", "camp");
  jvmud_add_action("queue_status", "queue");
  jvmud_add_action("list_routes", "routes");
  jvmud_add_action("board", "board");
  jvmud_add_action("party_command", "party");
  jvmud_invoke_lpc_object("system/socials", "register", 0);
}

int look(mixed target) {
  object place;
  object entity;
  string normalized_target;

  place = jvmud_entity_location(jvmud_current_lpc_object());
  if (!place) {
    write("You are nowhere in Avelorn.\n");
    return 1;
  }
  if (target) {
    normalized_target = jvmud_to_string(target);
    if (jvmud_size(normalized_target) > 3
        && jvmud_lowercase_text(jvmud_extract_text(normalized_target, 0, 2)) == "at ") {
      normalized_target = jvmud_extract_text(normalized_target, 3);
    }
    entity = jvmud_find_entity(normalized_target, place);
    if (!entity) {
      entity = jvmud_find_entity(normalized_target, jvmud_current_lpc_object());
    }
    if (!entity) {
      if (jvmud_method_exists("examine_detail", place)
          && jvmud_invoke_lpc_object(place, "examine_detail", normalized_target)) {
        return 1;
      }
      write("You do not see that here.\n");
      return 1;
    }
    jvmud_invoke_lpc_object(entity, "describe", jvmud_current_lpc_object());
    return 1;
  }
  jvmud_invoke_lpc_object(place, "describe", jvmud_current_lpc_object());
  jvmud_invoke_lpc_object("system/gmcp", "send_room", jvmud_current_lpc_object());
  return 1;
}

int query_inspect_hints() { return inspect_hints; }

int inspection_hints(mixed requested) {
  string choice;

  if (requested) {
    choice = jvmud_lowercase_text(requested);
    if (choice != "on" && choice != "off") {
      write("Use hints on or hints off.\n");
      return 1;
    }
    inspect_hints = choice == "on";
    save_character();
  }
  write("Inspection hints are " + (inspect_hints ? "on" : "off") + ".\n");
  write("Type details to list the scenery you can examine.\n");
  return 1;
}

int travel_to(string direction, string destination) {
  object old_place;
  object new_place;

  old_place = jvmud_entity_location(jvmud_current_lpc_object());
  if (!jvmud_invoke_lpc_object("system/world", "relocate", jvmud_current_lpc_object(), destination)) {
    return 0;
  }
  new_place = jvmud_entity_location(jvmud_current_lpc_object());
  if (!new_place || new_place == old_place) {
    return 0;
  }
  travel_dirty = 1;
  if (jvmud_method_exists("query_safe", new_place)
      && jvmud_invoke_lpc_object(new_place, "query_safe")) {
    sanctuary = destination;
  }
  if (old_place) {
    avelorn_emit_at(old_place, character_name + " leaves " + direction + ".\n");
  }
  avelorn_emit_except(
      jvmud_current_lpc_object(),
      character_name + " arrives.\n",
      jvmud_current_lpc_object());
  if (brief_mode) {
    if (jvmud_method_exists("describe_brief", new_place)) {
      jvmud_invoke_lpc_object(new_place, "describe_brief", jvmud_current_lpc_object());
    } else {
      describe_briefly(new_place);
    }
  } else {
    jvmud_invoke_lpc_object(new_place, "describe", jvmud_current_lpc_object());
  }
  jvmud_invoke_lpc_object("system/gmcp", "send_room", jvmud_current_lpc_object());
  return 1;
}

int walk_route(mixed argument) {
  object actor;
  mixed *directions;
  string direction;
  int count;
  int index;

  actor = jvmud_current_lpc_object();
  directions = argument ? jvmud_split_text(jvmud_lowercase_text(argument), " ") : ({ });
  count = jvmud_size(directions);
  if (!count || count > 8) {
    write("Use walk followed by up to eight exits, such as walk north east enter.\n");
    return 1;
  }
  if (count + jvmud_invoke_lpc_object("system/travel", "count", actor) > 8) {
    write("There is not enough room in your movement queue for that route.\n");
    return 1;
  }
  for (index = 0; index < count; index += 1) {
    direction = jvmud_invoke_lpc_object("system/world", "direction_name", directions[index]);
    if (!jvmud_invoke_lpc_object("system/travel", "enqueue", actor, direction, 1)) {
      break;
    }
  }
  return 1;
}

int flee(mixed argument) {
  object actor;
  object place;
  string direction;
  mixed edge;

  actor = jvmud_current_lpc_object();
  place = jvmud_entity_location(actor);
  if (!argument || !combat_target || !place) {
    write("Use flee <exit> to break away from combat.\n");
    return 1;
  }
  direction = jvmud_invoke_lpc_object("system/world", "direction_name", jvmud_lowercase_text(argument));
  edge = jvmud_invoke_lpc_object(place, "query_travel_step", direction);
  if (!jvmud_is_mapping(edge)) {
    write("You cannot flee that way.\n");
    return 1;
  }
  combat_target = 0;
  jvmud_invoke_lpc_object("system/input", "clear", actor);
  jvmud_invoke_lpc_object("system/travel", "clear", actor);
  jvmud_invoke_lpc_object("system/travel", "enqueue", actor, direction, 1);
  write("You break away and turn to flee.\n");
  return 1;
}

int camp(mixed ignored) {
  jvmud_invoke_lpc_object("system/travel", "camp", jvmud_current_lpc_object());
  return 1;
}

int queue_status(mixed ignored) {
  object actor;

  actor = jvmud_current_lpc_object();
  write("Command queue: " + jvmud_invoke_lpc_object("system/input", "count", actor) + "/16.\n");
  write(jvmud_invoke_lpc_object("system/travel", "status", actor) + "\n");
  return 1;
}

int query_travel_reserve() { return travel_reserve; }
int query_travel_blocked() { return !entered_world || combat_target != 0 || jvmud_size(transit_id) > 0; }

int list_routes(mixed ignored) {
  return jvmud_invoke_lpc_object("system/transit", "list_routes", jvmud_current_lpc_object());
}

int board(mixed destination) {
  return jvmud_invoke_lpc_object("system/transit", "board", jvmud_current_lpc_object(), destination);
}

int party_command(mixed command) {
  return jvmud_invoke_lpc_object("system/party", "command", jvmud_current_lpc_object(), command);
}

mixed query_transit() {
  if (!jvmud_size(transit_id)) {
    return 0;
  }
  return ([
    "id": transit_id,
    "destination": transit_destination,
    "arrives_at": transit_arrives_at
  ]);
}

int begin_transit(string id, string destination, int arrives_at, int fare) {
  if (query_travel_blocked() || fare < 0) {
    return 0;
  }
  if (copper < fare) {
    write("You do not have enough coin for that passage.\n");
    return 0;
  }
  transit_id = id;
  transit_destination = destination;
  transit_arrives_at = arrives_at;
  copper -= fare;
  if (!save_character()) {
    copper += fare;
    transit_id = "";
    transit_destination = "";
    transit_arrives_at = 0;
    return 0;
  }
  jvmud_move_entity(jvmud_current_lpc_object(), "place/in-transit");
  return 1;
}

int finish_transit(string id, string destination) {
  object actor;
  string old_destination;
  int old_arrival;

  if (id != transit_id || jvmud_time() < transit_arrives_at
      || !jvmud_invoke_lpc_object("system/world", "valid_place", destination)) {
    return 0;
  }
  actor = jvmud_current_lpc_object();
  old_destination = transit_destination;
  old_arrival = transit_arrives_at;
  if (!jvmud_invoke_lpc_object("system/world", "relocate", actor, destination)) {
    return 0;
  }
  transit_id = "";
  transit_destination = "";
  transit_arrives_at = 0;
  if (save_character()) {
    return 1;
  }
  transit_id = id;
  transit_destination = old_destination;
  transit_arrives_at = old_arrival;
  jvmud_move_entity(actor, "place/in-transit");
  return 0;
}

int adjust_travel_reserve(int delta) {
  travel_reserve += delta;
  if (travel_reserve > 100) { travel_reserve = 100; }
  if (travel_reserve < 0) { travel_reserve = 0; }
  travel_dirty = 1;
  return travel_reserve;
}

object travel_ration() {
  object item;

  item = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (item) {
    if (is_item(item) && jvmud_invoke_lpc_object(item, "query_blueprint") == "consumable/field-bread") {
      return item;
    }
    item = jvmud_next_entity_at(item);
  }
  return 0;
}

int query_has_travel_ration() { return travel_ration() != 0; }

int complete_travel_camp() {
  object ration;

  ration = travel_ration();
  if (!ration) {
    write("You have no ration left to finish your rest.\n");
    return 0;
  }
  jvmud_destroy_lpc_object(ration);
  adjust_travel_reserve(40);
  save_character();
  write("You eat your ration and recover. Travel reserve " + travel_reserve + "/100.\n");
  return 1;
}

int brief(mixed ignored) {
  brief_mode = !brief_mode;
  if (brief_mode) {
    write("Brief mode is on.\n");
  } else {
    write("Brief mode is off.\n");
  }
  save_character();
  return 1;
}

void describe_briefly(object place) {
  object actor;
  object entity;
  string occupants;
  string items;

  actor = jvmud_current_lpc_object();
  occupants = "";
  items = "";
  write(jvmud_invoke_lpc_object(place, "short") + "\n");
  entity = jvmud_first_entity_at(place);
  while (entity) {
    if (entity != actor) {
      if (jvmud_method_exists("query_blueprint", entity)
          && jvmud_method_exists("short", entity)) {
        items = append_brief_entry(
            items,
            jvmud_invoke_lpc_object(entity, "short"));
      } else if (jvmud_method_exists("query_name", entity)) {
        occupants = append_brief_entry(
            occupants,
            jvmud_invoke_lpc_object(entity, "query_name"));
      }
    }
    entity = jvmud_next_entity_at(entity);
  }
  write("Occupants: " + brief_value_or_none(occupants) + "\n");
  write("Items: " + brief_value_or_none(items) + "\n");
  write("Exits: " + brief_value_or_none(brief_exits(place)) + "\n");
}

string brief_exits(object place) {
  string exits;

  if (jvmud_method_exists("query_brief_exits", place)) {
    return jvmud_invoke_lpc_object(place, "query_brief_exits");
  }
  exits = "";
  if (jvmud_method_exists("north", place)) {
    exits = append_brief_token(exits, "n");
  }
  if (jvmud_method_exists("east", place)) {
    exits = append_brief_token(exits, "e");
  }
  if (jvmud_method_exists("south", place)) {
    exits = append_brief_token(exits, "s");
  }
  if (jvmud_method_exists("west", place)) {
    exits = append_brief_token(exits, "w");
  }
  if (jvmud_method_exists("up", place)) {
    exits = append_brief_token(exits, "u");
  }
  if (jvmud_method_exists("down", place)) {
    exits = append_brief_token(exits, "d");
  }
  return exits;
}

string append_brief_entry(string entries, string entry) {
  if (jvmud_size(entries) == 0) {
    return entry;
  }
  return entries + ", " + entry;
}

string append_brief_token(string entries, string entry) {
  if (jvmud_size(entries) == 0) {
    return entry;
  }
  return entries + " " + entry;
}

string brief_value_or_none(string value) {
  if (jvmud_size(value) == 0) {
    return "none";
  }
  return value;
}

int score(mixed ignored) {
  write(character_name + ", level " + level + " " + character_class + "\n");
  write("Health " + health + "/" + max_health + "  ");
  write(resource_name() + " " + resource + "/" + max_resource + "\n");
  if (level < level_cap()) {
    write("Experience " + experience + "/" + experience_for_next_level() + "\n");
  } else {
    write("Experience: level cap reached\n");
  }
  write("Strength " + strength + "  Dexterity " + dexterity);
  write("  Constitution " + constitution + "\n");
  write("Intelligence " + intelligence + "  Wisdom " + wisdom);
  write("  Charisma " + charisma + "\n");
  write("Unspent attribute points: " + attribute_points + "\n");
  write("Coin: " + money_text() + "\n");
  send_gmcp_character();
  return 1;
}

int inventory(mixed ignored) {
  object item;
  int total_weight;

  item = jvmud_first_entity_at(jvmud_current_lpc_object());
  if (!item) {
    write("You are carrying nothing.\n");
    return 1;
  }
  write("You are carrying:\n");
  while (item) {
    if (jvmud_method_exists("query_blueprint", item)) {
      write("  " + jvmud_invoke_lpc_object(item, "short") + "\n");
      total_weight += jvmud_invoke_lpc_object(item, "query_weight");
    }
    item = jvmud_next_entity_at(item);
  }
  write("Carried weight: " + total_weight + "/" + carry_capacity() + ".\n");
  return 1;
}

int equipment(mixed ignored) {
  object item;
  int found;

  write("Equipped:\n");
  item = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (item) {
    if (is_item(item) && jvmud_invoke_lpc_object(item, "query_equipped")) {
      write("  " + jvmud_invoke_lpc_object(item, "query_slot") + ": ");
      write(jvmud_invoke_lpc_object(item, "short") + "\n");
      found = 1;
    }
    item = jvmud_next_entity_at(item);
  }
  if (!found) {
    write("  Nothing.\n");
  }
  return 1;
}

int get_item(mixed target) {
  object place;
  object item;
  int item_weight;

  if (!target) {
    write("Get what?\n");
    return 1;
  }
  place = jvmud_entity_location(jvmud_current_lpc_object());
  item = jvmud_find_entity(target, place);
  if (!item || !is_item(item)) {
    write("You do not see a portable item by that name.\n");
    return 1;
  }
  item_weight = jvmud_invoke_lpc_object(item, "query_weight");
  if (carried_weight() + item_weight > carry_capacity()) {
    write("That would exceed your carrying capacity.\n");
    return 1;
  }
  jvmud_move_entity(item, jvmud_current_lpc_object());
  jvmud_invoke_lpc_object("system/atlas_places", "forget_drop", item);
  write("You take " + jvmud_invoke_lpc_object(item, "short") + ".\n");
  save_character();
  return 1;
}

int drop_item(mixed target) {
  object item;
  object place;

  if (!target) {
    write("Drop what?\n");
    return 1;
  }
  item = jvmud_find_entity(target, jvmud_current_lpc_object());
  if (!item || !is_item(item)) {
    write("You are not carrying that.\n");
    return 1;
  }
  if (jvmud_invoke_lpc_object(item, "query_equipped")) {
    write("Unequip it before dropping it.\n");
    return 1;
  }
  place = jvmud_entity_location(jvmud_current_lpc_object());
  jvmud_move_entity(item, place);
  jvmud_invoke_lpc_object("system/atlas_places", "note_drop", item, place);
  write("You drop " + jvmud_invoke_lpc_object(item, "short") + ".\n");
  if (jvmud_method_exists("atlas_path", place)) {
    write("Loose items weather away here after about two minutes.\n");
  }
  save_character();
  return 1;
}

int equip_item(mixed target) {
  object item;
  object other;
  string slot;
  string stat;
  int minimum;

  if (!target) {
    write("Equip what?\n");
    return 1;
  }
  item = jvmud_find_entity(target, jvmud_current_lpc_object());
  if (!item || !is_item(item)) {
    write("You are not carrying that item.\n");
    return 1;
  }
  slot = jvmud_invoke_lpc_object(item, "query_slot");
  if (slot == "none") {
    write("That item cannot be equipped.\n");
    return 1;
  }
  other = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (other) {
    if (other != item && is_item(other)
        && jvmud_invoke_lpc_object(other, "query_equipped")
        && jvmud_invoke_lpc_object(other, "query_slot") == slot) {
      jvmud_invoke_lpc_object(other, "set_equipped", 0);
    }
    other = jvmud_next_entity_at(other);
  }
  jvmud_invoke_lpc_object(item, "set_equipped", 1);
  write("You equip " + jvmud_invoke_lpc_object(item, "short") + ".\n");
  stat = jvmud_invoke_lpc_object(item, "query_governing_stat");
  minimum = jvmud_invoke_lpc_object(item, "query_minimum_stat");
  if (minimum > 0 && stat_value(stat) < minimum) {
    write("You can use it, but your " + stat + " is below the recommended " + minimum);
    write(", so it will be less effective.\n");
  }
  save_character();
  return 1;
}

int unequip_item(mixed target) {
  object item;

  if (!target) {
    write("Unequip what?\n");
    return 1;
  }
  item = jvmud_find_entity(target, jvmud_current_lpc_object());
  if (!item || !is_item(item) || !jvmud_invoke_lpc_object(item, "query_equipped")) {
    write("You do not have that equipped.\n");
    return 1;
  }
  jvmud_invoke_lpc_object(item, "set_equipped", 0);
  write("You unequip " + jvmud_invoke_lpc_object(item, "short") + ".\n");
  save_character();
  return 1;
}

int use_item(mixed target) {
  object actor;
  object item;
  int outcome;

  if (!target) {
    write("Use what?\n");
    return 1;
  }
  actor = jvmud_current_lpc_object();
  item = jvmud_find_entity(target, actor);
  if (!item || !is_item(item)) {
    write("You are not carrying that item.\n");
    return 1;
  }
  outcome = jvmud_invoke_lpc_object(item, "use_by", actor);
  if (!outcome) {
    write("You find no way to use that item here.\n");
    return 1;
  }
  if (outcome == 2) { jvmud_destroy_lpc_object(item); }
  save_character();
  send_gmcp_character();
  return 1;
}

int money(mixed ignored) {
  write("You carry " + money_text() + ".\n");
  return 1;
}

int attack(mixed target) {
  object place;
  object opponent;

  if (!target) {
    write("Attack what?\n");
    return 1;
  }
  place = jvmud_entity_location(jvmud_current_lpc_object());
  opponent = jvmud_find_entity(target, place);
  if (!opponent || !jvmud_method_exists("query_hostile", opponent)
      || !jvmud_invoke_lpc_object(opponent, "query_hostile")) {
    write("That is not a hostile combatant.\n");
    return 1;
  }
  if (combat_target == opponent) {
    write("You are already fighting "
        + jvmud_invoke_lpc_object(opponent, "query_name") + ".\n");
    return 1;
  }
  combat_target = opponent;
  jvmud_invoke_lpc_object("system/travel", "clear", jvmud_current_lpc_object());
  write("You engage " + jvmud_invoke_lpc_object(opponent, "query_name")
      + " in combat.\n");
  perform_combat_round();
  return 1;
}

void scheduled_update() {
  object actor;
  int commands;
  int movement;
  int arrived;

  if (!entered_world) {
    return;
  }
  actor = jvmud_current_lpc_object();
  jvmud_schedule_deferred_callback("scheduled_update", 1);
  combat_acted = 0;
  if (guard_ticks > 0) { guard_ticks -= 1; }
  arrived = jvmud_invoke_lpc_object("system/transit", "tick", actor);
  commands = jvmud_invoke_lpc_object("system/input", "drain", actor);
  movement = jvmud_invoke_lpc_object("system/travel", "drain", actor);
  if (combat_target) {
    perform_combat_round();
  }
  ticks_since_save += 1;
  if (travel_dirty && ticks_since_save >= 30) {
    save_character();
  }
  if (vitals_changed) {
    vitals_changed = 0;
    send_gmcp_character();
  }
  if (commands || movement || arrived) {
    jvmud_invoke_lpc_object("system/input", "prompt", actor);
  }
}

void perform_combat_round() {
  object place;
  object opponent;

  opponent = combat_target;
  if (!opponent || combat_acted) {
    return;
  }
  place = jvmud_entity_location(jvmud_current_lpc_object());
  if (!place
      || jvmud_entity_location(opponent) != place
      || !jvmud_method_exists("query_hostile", opponent)
      || !jvmud_invoke_lpc_object(opponent, "query_hostile")) {
    combat_target = 0;
    write("Your opponent is no longer within reach.\n");
    return;
  }
  combat_acted = 1;
  jvmud_invoke_lpc_object(
      opponent,
      "receive_attack",
      jvmud_current_lpc_object(),
      attack_damage());
}

int consider(mixed target) {
  object place;
  object opponent;
  int opponent_level;
  int difference;
  string opponent_gender;
  string subject_word;
  string be_word;

  if (!target) {
    write("Consider what?\n");
    return 1;
  }
  place = jvmud_entity_location(jvmud_current_lpc_object());
  opponent = jvmud_find_entity(target, place);
  if (!opponent || !jvmud_method_exists("query_hostile", opponent)
      || !jvmud_invoke_lpc_object(opponent, "query_hostile")) {
    write("That is not a hostile combatant.\n");
    return 1;
  }
  opponent_level = jvmud_invoke_lpc_object(opponent, "query_level");
  difference = opponent_level - level;
  opponent_gender = jvmud_invoke_lpc_object(opponent, "query_gender");
  subject_word = jvmud_invoke_lpc_object("system/pronouns", "subject", opponent_gender);
  be_word = jvmud_invoke_lpc_object("system/pronouns", "be_present", opponent_gender);
  write(jvmud_invoke_lpc_object(opponent, "query_name") + " is level ");
  write(opponent_level + ". ");
  if (difference >= 3) {
    write(jvmud_capitalize_text(subject_word) + " " + be_word);
    write(" far beyond your present training, though you may still engage ");
    write(jvmud_invoke_lpc_object("system/pronouns", "object_form", opponent_gender) + ".\n");
  } else if (difference > 0) {
    write(jvmud_capitalize_text(subject_word) + " " + be_word);
    write(" a dangerous challenge for you, but not an impossible one.\n");
  } else if (difference < -2) {
    write(jvmud_capitalize_text(subject_word) + " should pose little danger to you.\n");
  } else {
    write(jvmud_capitalize_text(subject_word) + " " + be_word);
    write(" an appropriate challenge for your present training.\n");
  }
  return 1;
}

mapping active_style() {
  mixed *names;
  mapping selected;

  selected = jvmud_invoke_lpc_object("system/combat_rules", "style", character_class, combat_style);
  if (selected) { return selected; }
  names = jvmud_invoke_lpc_object("system/combat_rules", "styles", character_class);
  return jvmud_invoke_lpc_object("system/combat_rules", "style", character_class, names[0]);
}

int select_style(mixed requested) {
  mixed *names;
  mapping profile;
  string id;
  int index;

  if (!requested) {
    write("Current style is " + active_style()["name"] + ". Choose style <name>.\n");
    names = jvmud_invoke_lpc_object("system/combat_rules", "styles", character_class);
    for (index = 0; index < jvmud_size(names); index += 1) {
      id = names[index];
      profile = jvmud_invoke_lpc_object("system/combat_rules", "style", character_class, id);
      write("  " + id + " — " + profile["description"] + "\n");
    }
    return 1;
  }
  if (combat_target) {
    write("Choose a style after breaking away from combat.\n");
    return 1;
  }
  id = jvmud_lowercase_text(requested);
  profile = jvmud_invoke_lpc_object("system/combat_rules", "style", character_class, id);
  if (!profile) {
    write("Use style to list your combat styles.\n");
    return 1;
  }
  combat_style = id;
  save_character();
  write("You adopt the " + profile["name"] + " style.\n");
  return 1;
}

int list_techniques(mixed ignored) {
  mixed *names;
  mapping technique;
  int index;

  names = jvmud_invoke_lpc_object("system/combat_rules", "techniques", character_class);
  write("Use ability <technique> [target]. Techniques:\n");
  for (index = 0; index < jvmud_size(names); index += 1) {
    technique = jvmud_invoke_lpc_object("system/combat_rules", "technique", character_class, names[index]);
    write("  " + names[index] + " (level " + technique["level"] + ", "
        + technique["cost"] + " " + resource_name() + ") — " + technique["description"] + "\n");
  }
  return 1;
}

int class_ability(mixed argument) {
  mapping technique;
  mapping style;
  mixed *names;
  object target;
  object actor;
  string name;
  string requested;
  string effect;
  int separator;
  int cost;
  int power;

  if (!argument) { return list_techniques(0); }
  if (combat_acted || jvmud_size(transit_id)) {
    write("You cannot use a technique right now.\n");
    return 1;
  }
  requested = jvmud_lowercase_text(argument);
  separator = jvmud_member(requested, ' ');
  name = separator < 0 ? requested : jvmud_extract_text(requested, 0, separator - 1);
  technique = jvmud_invoke_lpc_object("system/combat_rules", "technique", character_class, name);
  if (!technique) {
    names = jvmud_invoke_lpc_object("system/combat_rules", "techniques", character_class);
    technique = jvmud_invoke_lpc_object("system/combat_rules", "technique", character_class, names[0]);
  } else {
    requested = separator < 0 ? "" : jvmud_extract_text(requested, separator + 1);
  }
  if (level < technique["level"]) {
    write(technique["name"] + " becomes available at level " + technique["level"] + ".\n");
    return 1;
  }
  style = active_style();
  cost = technique["cost"] + style["cost"];
  if (resource < cost) {
    write("You need " + cost + " " + jvmud_lowercase_text(resource_name()) + ". Rest to recover.\n");
    return 1;
  }
  actor = jvmud_current_lpc_object();
  effect = technique["effect"];
  if (jvmud_size(technique["handler"])) {
    power = technique["power"] + style["support"];
    if (!jvmud_invoke_lpc_object(technique["handler"], "perform", actor, requested, power)) {
      return 1;
    }
  } else if (effect == "heal" || effect == "guard") {
    target = support_target(requested);
    if (!target) { return 1; }
    power = technique["power"] + style["support"];
    apply_support(target, effect, power);
  } else {
    target = jvmud_size(requested)
        ? jvmud_find_entity(requested, jvmud_entity_location(actor)) : combat_target;
    if (!target || !jvmud_method_exists("query_hostile", target)
        || !jvmud_invoke_lpc_object(target, "query_hostile")) {
      write("Choose a hostile combatant or use abilities for technique help.\n");
      return 1;
    }
    combat_target = target;
    jvmud_invoke_lpc_object("system/travel", "clear", actor);
    if (effect == "stagger") {
      jvmud_invoke_lpc_object(target, "interrupt_attack");
    }
    jvmud_invoke_lpc_object(target, "receive_attack", actor,
        attack_damage() + technique["power"] + level);
  }
  combat_acted = 1;
  resource -= cost;
  vitals_changed = 1;
  travel_dirty = 1;
  write("You use " + technique["name"] + ".\n");
  return 1;
}

object support_target(string requested) {
  object actor;
  object target;

  actor = jvmud_current_lpc_object();
  if (!jvmud_size(requested) || requested == "self") { return actor; }
  target = jvmud_invoke_lpc_object("system/party", "nearby_member", actor, requested);
  if (!target) {
    write("Choose yourself or a nearby member of your party.\n");
    return 0;
  }
  return target;
}

void apply_support(object target, string effect, int power) {
  object opponent;
  int helped;

  if (effect == "guard") {
    helped = jvmud_invoke_lpc_object(target, "receive_guard", power);
  } else {
    helped = jvmud_invoke_lpc_object(target, "receive_healing", power);
  }
  opponent = jvmud_invoke_lpc_object(target, "query_combat_target");
  if (helped && opponent) {
    jvmud_invoke_lpc_object(opponent, "remember_contributor", jvmud_current_lpc_object());
  }
}

object query_combat_target() { return combat_target; }

int receive_guard(int power) {
  if (guard_ticks > 0 && guard_power >= power) { return 0; }
  guard_power = power;
  guard_ticks = 3;
  avelorn_write_to(jvmud_current_lpc_object(), "You are braced for the next strike.\n");
  return 1;
}

int receive_healing(int power) {
  int restored;

  restored = max_health - health;
  if (restored > power) { restored = power; }
  if (restored <= 0) { return 0; }
  health += restored;
  vitals_changed = 1;
  travel_dirty = 1;
  avelorn_write_to(jvmud_current_lpc_object(), "You recover " + restored + " health.\n");
  return restored;
}

int brace(mixed ignored) {
  if (!combat_target || combat_acted) {
    write("Brace during combat when you have time to react.\n");
    return 1;
  }
  combat_acted = 1;
  receive_guard(3 + level / 2);
  return 1;
}

int receive_damage(int amount, string attacker_name) {
  object actor;
  int protection;
  int lost_copper;

  actor = jvmud_current_lpc_object();
  protection = armor_protection();
  if (protection > amount / 2) { protection = amount / 2; }
  amount -= protection;
  if (guard_ticks > 0) {
    amount -= guard_power;
    guard_ticks = 0;
  }
  if (amount < 1) {
    amount = 1;
  }
  health -= amount;
  vitals_changed = 1;
  travel_dirty = 1;
  avelorn_write_to(actor, attacker_name + " strikes you for " + amount + " damage.\n");
  if (health > 0) {
    avelorn_write_to(actor, "You have " + health + "/" + max_health + " health remaining.\n");
    return health;
  }

  avelorn_emit_except(
      jvmud_current_lpc_object(),
      character_name + " is overcome and carried to safety.\n",
      jvmud_current_lpc_object());
  combat_target = 0;
  jvmud_invoke_lpc_object("system/input", "clear", actor);
  jvmud_invoke_lpc_object("system/travel", "clear", actor);
  jvmud_invoke_lpc_object("system/party", "stop_following", actor, "");
  jvmud_invoke_lpc_object("system/party", "stop_followers", actor);
  lost_copper = copper / 10;
  copper -= lost_copper;
  recalculate_resources(1);
  saved_place = jvmud_invoke_lpc_object("system/world", "valid_place", sanctuary)
      ? sanctuary : starting_place();
  jvmud_invoke_lpc_object("system/world", "relocate", actor, saved_place);
  avelorn_write_to(actor, "You are overcome and carried to shelter.\n");
  if (lost_copper > 0) {
    avelorn_write_to(actor, "You lost "
        + jvmud_invoke_lpc_object("system/economy", "format_money", lost_copper)
        + " in the retreat.\n");
  }
  save_character();
  jvmud_dispatch_entity_command(actor, "look");
  return 0;
}

string starting_place() {
  return jvmud_invoke_lpc_object("system/world", "starting_place");
}

void award_victory(int xp, int coins, string opponent_name, object opponent) {
  if (combat_target == opponent) {
    combat_target = 0;
  }
  avelorn_write_to(
      jvmud_current_lpc_object(),
      "You help defeat " + opponent_name + ".\n");
  if (coins > 0) {
    copper += coins;
    avelorn_write_to(
        jvmud_current_lpc_object(),
        "The recovered bounty is worth "
            + jvmud_invoke_lpc_object("system/economy", "format_money", coins) + ".\n");
  }
  gain_experience(xp);
  save_character();
}

int offer_quest(string quest_id) {
  int stage;
  int recommended_level;
  string prerequisite;
  mapping flags;

  materialize_quests();
  if (!jvmud_invoke_lpc_object("system/quests", "known", quest_id)) {
    write("There is no assignment available here.\n");
    return 1;
  }
  prerequisite = jvmud_invoke_lpc_object("system/quests", "prerequisite", quest_id);
  if (jvmud_size(prerequisite) && quest_stage(prerequisite) != 3) {
    write("Complete " + quest_title(prerequisite) + " first.\n");
    return 1;
  }
  stage = quest_stage(quest_id);
  if (stage == 3) {
    flags = quest_flags[quest_id];
    if (!jvmud_invoke_lpc_object("system/quests", "repeatable", quest_id)
        || (flags && flags["claimed_rotation"] == jvmud_invoke_lpc_object("system/quests", "current_rotation"))) {
      write("You have already completed " + quest_title(quest_id) + ".\n");
      return 1;
    }
    quest_flags[quest_id] = ([ ]);
    stage = 0;
  }
  if (stage > 0) {
    write("That assignment is already recorded in your journal.\n");
    return 1;
  }
  recommended_level = jvmud_invoke_lpc_object("system/quests", "recommended_level", quest_id);
  write(quest_title(quest_id) + " (recommended level " + recommended_level + ")\n");
  write(jvmud_invoke_lpc_object("system/quests", "description", quest_id) + "\n");
  if (level < recommended_level) {
    write("This is beyond your current training, but the attempt is not prohibited.\n");
  }
  quest_stages[quest_id] = 1;
  quest_counts[quest_id] = 0;
  write("You accept the assignment.\n");
  save_character();
  return 1;
}

void record_quest_defeat(string quest_tag) {
  string quest_id;
  string flag_id;
  mapping flags;
  int required;
  int count;

  if (!quest_tag || jvmud_size(quest_tag) == 0) {
    return;
  }
  materialize_quests();
  quest_id = jvmud_invoke_lpc_object("system/quests", "quest_for_defeat_tag", quest_tag);
  if (!quest_id || quest_stage(quest_id) != 1) {
    return;
  }
  if (!jvmud_invoke_lpc_object("system/quests", "repeatable_defeat_tag", quest_tag)) {
    if (jvmud_member(quest_flags, quest_id)) {
      flags = quest_flags[quest_id];
    } else {
      flags = ([ ]);
    }
    flag_id = "defeat:" + quest_tag;
    if (jvmud_member(flags, flag_id)) {
      return;
    }
    flags[flag_id] = 1;
    quest_flags[quest_id] = flags;
  }
  count = quest_count(quest_id) + 1;
  required = jvmud_invoke_lpc_object("system/quests", "required_count", quest_id);
  quest_counts[quest_id] = count;
  avelorn_write_to(
      jvmud_current_lpc_object(),
      quest_title(quest_id) + ": " + count + "/" + required + ".\n");
  if (count >= required) {
    quest_stages[quest_id] = 2;
    avelorn_write_to(
        jvmud_current_lpc_object(),
        "The assignment is ready to report.\n");
  }
  save_character();
}

int record_quest_action(string action_tag) {
  string quest_id;
  mapping flags;
  int count;
  int required;

  materialize_quests();
  quest_id = jvmud_invoke_lpc_object("system/quests", "quest_for_action_tag", action_tag);
  if (!quest_id || quest_stage(quest_id) == 0 || quest_stage(quest_id) == 3) {
    write("You make a careful inspection, but no current assignment concerns it.\n");
    return 1;
  }
  if (jvmud_member(quest_flags, quest_id)) {
    flags = quest_flags[quest_id];
  } else {
    flags = ([ ]);
  }
  if (jvmud_member(flags, action_tag)) {
    write("You have already recorded this objective.\n");
    return 1;
  }
  if (quest_stage(quest_id) != 1) {
    write("That assignment is already ready to report.\n");
    return 1;
  }
  flags[action_tag] = 1;
  quest_flags[quest_id] = flags;
  count = quest_count(quest_id) + 1;
  quest_counts[quest_id] = count;
  required = jvmud_invoke_lpc_object("system/quests", "required_count", quest_id);
  write(quest_title(quest_id) + ": " + count + "/" + required + ".\n");
  if (count >= required) {
    quest_stages[quest_id] = 2;
    write("The assignment is ready to report.\n");
  }
  save_character();
  return 1;
}

int turn_in_quest(string quest_id) {
  object reward_item;
  mapping previous_flags;
  mapping flags;
  string blueprint;
  int *progress;
  int previous_rotation;
  int had_rotation;
  int stage;
  int xp;
  int coins;

  materialize_quests();
  stage = quest_stage(quest_id);
  if (stage == 0) {
    write("You have not accepted that assignment. Ask for work first.\n");
    return 1;
  }
  if (stage == 1) {
    write("The assignment is not yet complete.\n");
    return 1;
  }
  if (stage == 3) {
    write("That assignment has already been reported and rewarded.\n");
    return 1;
  }

  blueprint = jvmud_invoke_lpc_object(
      "system/quests", "reward_for_class", quest_id, character_class);
  if (jvmud_size(blueprint)) {
    reward_item = jvmud_invoke_lpc_object("system/items", "create", blueprint);
    if (!reward_item) {
      write("The assignment reward is unavailable. Try reporting later.\n");
      return 1;
    }
    jvmud_move_entity(reward_item, jvmud_current_lpc_object());
  }
  progress = ({ level, experience, attribute_points, health, max_health, resource, max_resource });
  quest_stages[quest_id] = 3;
  if (jvmud_invoke_lpc_object("system/quests", "repeatable", quest_id)) {
    previous_flags = quest_flags[quest_id];
    flags = previous_flags ? previous_flags : ([ ]);
    had_rotation = jvmud_member(flags, "claimed_rotation");
    previous_rotation = flags["claimed_rotation"];
    flags["claimed_rotation"] = jvmud_invoke_lpc_object("system/quests", "current_rotation");
    quest_flags[quest_id] = flags;
  }
  xp = jvmud_invoke_lpc_object("system/quests", "experience_reward", quest_id);
  coins = jvmud_invoke_lpc_object("system/quests", "copper_reward", quest_id);
  copper += coins;
  apply_experience(xp);
  if (!save_character()) {
    if (reward_item) { jvmud_destroy_lpc_object(reward_item); }
    copper -= coins;
    level = progress[0];
    experience = progress[1];
    attribute_points = progress[2];
    health = progress[3];
    max_health = progress[4];
    resource = progress[5];
    max_resource = progress[6];
    quest_stages[quest_id] = 2;
    if (flags && had_rotation) {
      flags["claimed_rotation"] = previous_rotation;
    } else if (flags && previous_flags) {
      jvmud_mapping_delete(flags, "claimed_rotation");
    } else if (flags) {
      jvmud_mapping_delete(quest_flags, quest_id);
    }
    write("Your journal is still ready to report this assignment.\n");
    return 1;
  }
  write("You complete " + quest_title(quest_id) + ".\n");
  write("You receive "
      + jvmud_invoke_lpc_object("system/economy", "format_money", coins) + ".\n");
  announce_experience(xp, progress[0]);
  if (reward_item) {
    write("You receive " + jvmud_invoke_lpc_object(reward_item, "short") + ".\n");
  }
  return 1;
}

int quests(mixed ignored) {
  mixed *ids;
  int index;
  int stage;
  string quest_id;

  materialize_quests();
  ids = jvmud_mapping_keys(quest_stages);
  if (jvmud_size(ids) == 0) {
    write("Your journal contains no assignments.\n");
    return 1;
  }
  write("Assignments:\n");
  index = 0;
  while (index < jvmud_size(ids)) {
    quest_id = ids[index];
    stage = quest_stage(quest_id);
    write("  " + quest_title(quest_id) + " — " + quest_stage_text(stage));
    if (stage == 1 || stage == 2) {
      write(" (" + quest_count(quest_id) + "/");
      write(jvmud_invoke_lpc_object("system/quests", "required_count", quest_id) + ")");
    }
    write("\n");
    index += 1;
  }
  return 1;
}

int purchase_blueprint(string blueprint) {
  object item;
  int price;
  int item_weight;

  item = jvmud_invoke_lpc_object("system/items", "create", blueprint);
  if (!item) {
    write("That stock entry is unavailable.\n");
    return 1;
  }
  price = jvmud_invoke_lpc_object(item, "query_value");
  item_weight = jvmud_invoke_lpc_object(item, "query_weight");
  if (copper < price) {
    write("You do not have enough coin.\n");
    jvmud_destroy_lpc_object(item);
    return 1;
  }
  if (carried_weight() + item_weight > carry_capacity()) {
    write("You cannot carry that much weight.\n");
    jvmud_destroy_lpc_object(item);
    return 1;
  }
  copper -= price;
  jvmud_move_entity(item, jvmud_current_lpc_object());
  write("You buy " + jvmud_invoke_lpc_object(item, "short") + " for ");
  write(jvmud_invoke_lpc_object("system/economy", "format_money", price) + ".\n");
  save_character();
  return 1;
}

int sell_item(mixed target) {
  object item;
  int sale_price;
  string item_name;

  item = jvmud_find_entity(target, jvmud_current_lpc_object());
  if (!item || !is_item(item)) {
    write("You are not carrying that item.\n");
    return 1;
  }
  if (jvmud_invoke_lpc_object(item, "query_equipped")) {
    write("Unequip it before selling it.\n");
    return 1;
  }
  sale_price = jvmud_invoke_lpc_object(item, "query_value") / 2;
  item_name = jvmud_invoke_lpc_object(item, "short");
  jvmud_destroy_lpc_object(item);
  copper += sale_price;
  write("You sell " + item_name + " for ");
  write(jvmud_invoke_lpc_object("system/economy", "format_money", sale_price) + ".\n");
  save_character();
  return 1;
}

int improve(mixed target) {
  string stat;

  if (!target) {
    write("Improve which attribute?\n");
    return 1;
  }
  if (attribute_points <= 0) {
    write("You have no unspent attribute points.\n");
    return 1;
  }
  stat = jvmud_lowercase_text(target);
  if (stat == "strength") {
    strength += 1;
  } else if (stat == "dexterity") {
    dexterity += 1;
  } else if (stat == "constitution") {
    constitution += 1;
  } else if (stat == "intelligence") {
    intelligence += 1;
  } else if (stat == "wisdom") {
    wisdom += 1;
  } else if (stat == "charisma") {
    charisma += 1;
  } else {
    write("Choose strength, dexterity, constitution, intelligence, wisdom, or charisma.\n");
    return 1;
  }
  attribute_points -= 1;
  recalculate_resources(0);
  write("Your " + stat + " improves to " + stat_value(stat) + ".\n");
  save_character();
  send_gmcp_character();
  return 1;
}

int complete_introductory_drill() {
  if (introductory_drill_completed) {
    write("You have already completed the introductory drill.\n");
    return 1;
  }
  introductory_drill_completed = 1;
  write("You complete a measured course of footwork, signals, and field discipline.\n");
  gain_experience(100);
  save_character();
  return 1;
}

int rest_at_shrine() {
  if (combat_target) {
    write("You cannot rest during combat.\n");
    return 1;
  }
  sanctuary = jvmud_lpc_object_id(jvmud_entity_location(jvmud_current_lpc_object()));
  adjust_travel_reserve(100);
  recalculate_resources(1);
  write("You rest and recover your health, travel reserve, and "
      + jvmud_lowercase_text(resource_name()) + ".\n");
  save_character();
  send_gmcp_character();
  return 1;
}

int pronouns_command(mixed ignored) {
  string subject_word;
  string be_word;
  string have_word;

  subject_word = pronoun("subject");
  be_word = pronoun("be_present");
  have_word = pronoun("have_present");
  write(character_name + " uses " + subject_word + "/" + pronoun("object_form") + " pronouns.\n");
  write(jvmud_capitalize_text(subject_word) + " " + be_word);
  write(" an adventurer, and " + subject_word + " " + have_word);
  write(" begun " + pronoun("possessive_adjective") + " adventuring career.\n");
  return 1;
}

int who(mixed ignored) {
  object *connected;
  object persona;
  string name;
  int index;
  int online;

  connected = jvmud_users();
  index = 0;
  while (index < jvmud_size(connected)) {
    if (is_online_adventurer(connected[index])) {
      online += 1;
    }
    index += 1;
  }

  write("Adventurers (" + online + " online):\n");
  index = 0;
  while (index < jvmud_size(connected)) {
    persona = connected[index];
    if (is_online_adventurer(persona)) {
      name = jvmud_invoke_lpc_object(persona, "query_name");
      write("  " + name + " - level ");
      write(jvmud_invoke_lpc_object(persona, "query_level") + " ");
      write(jvmud_invoke_lpc_object(persona, "query_class") + " (");
      write(jvmud_invoke_lpc_object(persona, "pronoun", "subject") + "/");
      write(jvmud_invoke_lpc_object(persona, "pronoun", "object_form") + ")\n");
    }
    index += 1;
  }
  return 1;
}

int say(mixed value) {
  string message;

  if (!value || jvmud_size(value) == 0) {
    write("Say what?\n");
    return 1;
  }
  message = jvmud_to_string(value);
  write("You say, \"" + message + "\"\n");
  avelorn_emit_except(
      jvmud_current_lpc_object(),
      character_name + " says, \"" + message + "\"\n",
      jvmud_current_lpc_object());
  return 1;
}

int tell(mixed value) {
  object *connected;
  object persona;
  object target;
  string input;
  string lowered_input;
  string message;
  string name;
  string lowered_name;
  int index;
  int name_length;
  int target_name_length;

  if (!value || jvmud_size(value) == 0) {
    write("Tell whom what?\n");
    return 1;
  }

  input = jvmud_to_string(value);
  lowered_input = jvmud_lowercase_text(input);
  connected = jvmud_users();
  index = 0;
  while (index < jvmud_size(connected)) {
    persona = connected[index];
    if (is_online_adventurer(persona)) {
      name = jvmud_invoke_lpc_object(persona, "query_name");
      lowered_name = jvmud_lowercase_text(name);
      name_length = jvmud_size(name);
      if (lowered_input == lowered_name
          || (jvmud_size(input) > name_length
              && jvmud_extract_text(lowered_input, 0, name_length - 1) == lowered_name
              && lowered_input[name_length] == ' ')) {
        if (!target || name_length > target_name_length) {
          target = persona;
          target_name_length = name_length;
          if (jvmud_size(input) > name_length) {
            message = jvmud_extract_text(input, name_length + 1);
          } else {
            message = "";
          }
        }
      }
    }
    index += 1;
  }

  if (!target) {
    write("No adventurer by that name is presently in Avelorn.\n");
    return 1;
  }
  name = jvmud_invoke_lpc_object(target, "query_name");
  if (jvmud_size(message) == 0) {
    write("Tell " + name + " what?\n");
    return 1;
  }
  if (target == jvmud_current_lpc_object()) {
    write("You consider telling yourself, then think better of it.\n");
    return 1;
  }

  write("You tell " + name + ", \"" + message + "\"\n");
  avelorn_write_to(
      target,
      character_name + " tells you, \"" + message + "\"\n");
  return 1;
}

int is_online_adventurer(object persona) {
  if (!persona
      || !jvmud_method_exists("query_name", persona)
      || !jvmud_method_exists("query_class", persona)
      || !jvmud_method_exists("query_level", persona)) {
    return 0;
  }
  return jvmud_size(jvmud_invoke_lpc_object(persona, "query_name")) > 0
      && jvmud_size(jvmud_invoke_lpc_object(persona, "query_class")) > 0;
}

int help(mixed ignored) {
  write("Avelorn commands:\n");
  write("  look/examine, north/east/south/west, brief, score, pronouns, who\n");
  write("  say <message>, tell <character> <message>\n");
  write("  inventory, equipment, get, drop, equip, unequip, use\n");
  write("  attack <target> begins timed combat; ability uses a technique\n");
  write("  consider, quests, money, list, buy, sell\n");
  write("  train, improve, emotes, save, quit, help\n");
  write("  walk <exits>, flee <exit>, camp, rest, queue, stop\n");
  write("  routes lists departures; board <destination> takes a passage\n");
  write("  trails shows marked walks; notices lists local assignments\n");
  write("  details lists scenery; hints on marks it in room prose\n");
  write("  work to take an assignment, report to claim its reward\n");
  return 1;
}

int quit(mixed ignored) {
  end_session();
  write("Farewell.\n");
  jvmud_destroy_lpc_object(jvmud_current_lpc_object());
  return 1;
}

int save_command(mixed ignored) {
  if (save_character()) {
    write("Your character has been saved.\n");
  }
  return 1;
}

string pronoun(string form) {
  return jvmud_invoke_lpc_object("system/pronouns", form, gender);
}

string query_name() {
  return character_name;
}

string query_gender() {
  return gender;
}

string query_race() {
  return character_race;
}

string query_guild() {
  return jvmud_size(guild_id) ? guild_id : "";
}

int join_guild(string id) {
  string previous;

  if (!entered_world || combat_target || jvmud_size(guild_id)
      || !jvmud_invoke_lpc_object("system/guilds", "eligible", id, jvmud_current_lpc_object())) {
    return 0;
  }
  previous = guild_id;
  guild_id = id;
  if (save_character()) { return 1; }
  guild_id = previous;
  return 0;
}

int leave_guild() {
  string previous;

  if (!entered_world || combat_target || !jvmud_size(guild_id)) { return 0; }
  previous = guild_id;
  guild_id = "";
  if (save_character()) { return 1; }
  guild_id = previous;
  return 0;
}

string query_class() {
  return character_class;
}

int query_level() {
  return level;
}

void grant_starter_kit() {
  object item;
  string weapon;

  weapon = class_profile()["starter_weapon"];
  item = jvmud_invoke_lpc_object("system/items", "create", weapon);
  jvmud_invoke_lpc_object(item, "set_equipped", 1);
  jvmud_move_entity(item, jvmud_current_lpc_object());

  item = jvmud_invoke_lpc_object("system/items", "create", "armor/travel-cloak");
  jvmud_invoke_lpc_object(item, "set_equipped", 1);
  jvmud_move_entity(item, jvmud_current_lpc_object());

  item = jvmud_invoke_lpc_object("system/items", "create", "consumable/healing-draught");
  jvmud_move_entity(item, jvmud_current_lpc_object());
}

void snapshot_inventory() {
  mixed *states;
  mapping state;
  object item;

  states = ({ });
  item = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (item) {
    if (is_item(item)) {
      state = ([
        "blueprint": jvmud_invoke_lpc_object(item, "query_blueprint"),
        "equipped": jvmud_invoke_lpc_object(item, "query_equipped")
      ]);
      states += ({ state });
    }
    item = jvmud_next_entity_at(item);
  }
  inventory_state = jvmud_serialize_lpc_value(states);
}

void materialize_inventory() {
  mixed decoded;
  mixed *states;
  mapping state;
  object item;
  int index;

  if (inventory_materialized) {
    return;
  }
  inventory_materialized = 1;
  if (!inventory_state || jvmud_size(inventory_state) == 0) {
    return;
  }
  decoded = jvmud_deserialize_lpc_value(inventory_state);
  if (!jvmud_is_array(decoded)) {
    return;
  }
  states = decoded;
  index = 0;
  while (index < jvmud_size(states)) {
    state = states[index];
    if (jvmud_is_mapping(state)) {
      item = jvmud_invoke_lpc_object("system/items", "create", state["blueprint"]);
      if (item) {
        jvmud_invoke_lpc_object(item, "set_equipped", state["equipped"]);
        jvmud_move_entity(item, jvmud_current_lpc_object());
      }
    }
    index += 1;
  }
}

void clear_materialized_inventory() {
  object item;

  item = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (item) {
    jvmud_destroy_lpc_object(item);
    item = jvmud_first_entity_at(jvmud_current_lpc_object());
  }
  inventory_materialized = 0;
}

void snapshot_quests() {
  mapping state;

  state = ([
    "stages": quest_stages,
    "counts": quest_counts,
    "flags": quest_flags
  ]);
  quest_state = jvmud_serialize_lpc_value(state);
}

void materialize_quests() {
  mixed decoded;
  mapping state;

  if (quests_materialized) {
    return;
  }
  quests_materialized = 1;
  quest_stages = ([ ]);
  quest_counts = ([ ]);
  quest_flags = ([ ]);
  if (!quest_state || jvmud_size(quest_state) == 0) {
    return;
  }
  decoded = jvmud_deserialize_lpc_value(quest_state);
  if (!jvmud_is_mapping(decoded)) {
    return;
  }
  state = decoded;
  if (jvmud_is_mapping(state["stages"])) {
    quest_stages = state["stages"];
  }
  if (jvmud_is_mapping(state["counts"])) {
    quest_counts = state["counts"];
  }
  if (jvmud_is_mapping(state["flags"])) {
    quest_flags = state["flags"];
  }
}

int quest_stage(string quest_id) {
  if (!jvmud_member(quest_stages, quest_id)) {
    return 0;
  }
  return quest_stages[quest_id];
}

int quest_count(string quest_id) {
  if (!jvmud_member(quest_counts, quest_id)) {
    return 0;
  }
  return quest_counts[quest_id];
}

string quest_title(string quest_id) {
  return jvmud_invoke_lpc_object("system/quests", "title", quest_id);
}

string quest_stage_text(int stage) {
  if (stage == 1) {
    return "active";
  }
  if (stage == 2) {
    return "ready to report";
  }
  if (stage == 3) {
    return "complete";
  }
  return "unknown";
}

int is_item(object candidate) {
  return candidate && jvmud_method_exists("query_blueprint", candidate);
}

int carried_weight() {
  object item;
  int total;

  item = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (item) {
    if (is_item(item)) {
      total += jvmud_invoke_lpc_object(item, "query_weight");
    }
    item = jvmud_next_entity_at(item);
  }
  return total;
}

int attack_damage() {
  object item;
  mapping style;
  int base;
  int weapon_power;
  int penalty;
  int recommended;
  int minimum;
  string stat;

  base = stat_value(class_profile()["primary_stat"]);
  item = equipped_weapon();
  if (!item) {
    penalty = 3;
  } else {
    weapon_power = jvmud_invoke_lpc_object(item, "query_power");
    recommended = jvmud_invoke_lpc_object(item, "query_recommended_level");
    if (recommended > level) {
      penalty += recommended - level;
    }
    stat = jvmud_invoke_lpc_object(item, "query_governing_stat");
    minimum = jvmud_invoke_lpc_object(item, "query_minimum_stat");
    if (minimum > stat_value(stat)) {
      penalty += minimum - stat_value(stat);
    }
  }
  style = active_style();
  base = 3 + level + base / 3 + jvmud_random(4) + weapon_power + style["attack"] - penalty;
  if (base < 1) {
    return 1;
  }
  return base;
}

int query_concord_weapon() {
  object weapon;

  weapon = equipped_weapon();
  return weapon && jvmud_invoke_lpc_object(weapon, "query_concord");
}

int armor_protection() {
  object item;
  int protection;

  protection = active_style()["defense"];
  item = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (item) {
    if (is_item(item) && jvmud_invoke_lpc_object(item, "query_equipped")) {
      protection += jvmud_invoke_lpc_object(item, "query_protection");
    }
    item = jvmud_next_entity_at(item);
  }
  return protection;
}

object equipped_weapon() {
  object item;

  item = jvmud_first_entity_at(jvmud_current_lpc_object());
  while (item) {
    if (is_item(item)
        && jvmud_invoke_lpc_object(item, "query_equipped")
        && jvmud_invoke_lpc_object(item, "query_slot") == "weapon") {
      return item;
    }
    item = jvmud_next_entity_at(item);
  }
  return 0;
}

int carry_capacity() {
  return 20 + strength * 2;
}

string money_text() {
  return jvmud_invoke_lpc_object("system/economy", "format_money", copper);
}

void gain_experience(int amount) {
  int previous_level;

  previous_level = level;
  apply_experience(amount);
  announce_experience(amount, previous_level);
}

void apply_experience(int amount) {
  int cap;

  cap = level_cap();
  if (amount <= 0 || level >= cap) {
    return;
  }
  experience += amount;
  while (level < cap && experience >= experience_for_next_level()) {
    experience -= experience_for_next_level();
    level += 1;
    attribute_points += training_points_for(level);
    recalculate_resources(1);
  }
  if (level == cap) { experience = 0; }
}

void announce_experience(int amount, int previous_level) {
  int gained;

  if (amount <= 0 || previous_level >= level_cap()) { return; }
  avelorn_write_to(jvmud_current_lpc_object(), "You gain " + amount + " experience.\n");
  for (gained = previous_level + 1; gained <= level; gained += 1) {
    avelorn_write_to(
        jvmud_current_lpc_object(),
        "You advance to level " + gained + "!\n");
    if (training_points_for(gained)) {
      avelorn_write_to(
          jvmud_current_lpc_object(),
          "You earn an attribute point. Use improve <attribute>.\n");
    }
  }
  vitals_changed = 1;
}

int experience_for_next_level() {
  return jvmud_invoke_lpc_object("system/progression", "experience_to_advance", level);
}

int level_cap() {
  return jvmud_invoke_lpc_object("system/progression", "cap");
}

int training_points_for(int attained_level) {
  return jvmud_invoke_lpc_object("system/progression", "training_points_for", attained_level);
}

mapping class_profile() {
  return jvmud_invoke_lpc_object("system/combat_rules", "profile", character_class);
}

void recalculate_resources(int fill) {
  mapping profile;
  mapping weights;
  mixed *stats;
  int index;

  max_health = 30 + constitution * 4 + level * 6;
  profile = class_profile()["resource"];
  weights = profile["per_stat"];
  stats = jvmud_mapping_keys(weights);
  max_resource = profile["base"] + level * profile["per_level"];
  for (index = 0; index < jvmud_size(stats); index += 1) {
    max_resource += stat_value(stats[index]) * weights[stats[index]];
  }
  if (fill || health <= 0) {
    health = max_health;
  } else if (health > max_health) {
    health = max_health;
  }
  if (fill) {
    resource = max_resource;
  } else if (resource < 0) {
    resource = 0;
  } else if (resource > max_resource) {
    resource = max_resource;
  }
}

string resource_name() {
  return class_profile()["resource"]["name"];
}

int stat_value(string stat) {
  if (stat == "strength") {
    return strength;
  }
  if (stat == "dexterity") {
    return dexterity;
  }
  if (stat == "constitution") {
    return constitution;
  }
  if (stat == "intelligence") {
    return intelligence;
  }
  if (stat == "wisdom") {
    return wisdom;
  }
  if (stat == "charisma") {
    return charisma;
  }
  return 0;
}

void configure_starting_attributes() {
  mapping values;

  values = class_profile()["starting_attributes"];
  strength = starting_attribute(values, "strength");
  dexterity = starting_attribute(values, "dexterity");
  constitution = starting_attribute(values, "constitution");
  intelligence = starting_attribute(values, "intelligence");
  wisdom = starting_attribute(values, "wisdom");
  charisma = starting_attribute(values, "charisma");
}

int starting_attribute(mapping values, string attribute) {
  return jvmud_member(values, attribute) ? values[attribute] : 10;
}

string normalize_account_id(mixed value) {
  if (!value) {
    return "";
  }
  return jvmud_lowercase_text(value);
}

int valid_account_id(string value) {
  int index;
  int ch;

  if (jvmud_size(value) < 3 || jvmud_size(value) > 24) {
    return 0;
  }
  index = 0;
  while (index < jvmud_size(value)) {
    ch = value[index];
    if (!((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')
        || ch == '_' || ch == '-')) {
      return 0;
    }
    index = index + 1;
  }
  return 1;
}

int valid_character_name(mixed value) {
  int index;
  int ch;
  int saw_letter;

  if (!value || jvmud_size(value) < 2 || jvmud_size(value) > 24) {
    return 0;
  }
  index = 0;
  while (index < jvmud_size(value)) {
    ch = value[index];
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      saw_letter = 1;
    } else if (ch != ' ' && ch != '\'' && ch != '-') {
      return 0;
    }
    index = index + 1;
  }
  return saw_letter;
}

int valid_gender(string value) {
  return value == "male" || value == "female";
}

int valid_class(string value) {
  return jvmud_invoke_lpc_object("system/combat_rules", "known", value);
}

mixed password_problem(mixed value) {
  int index;
  int ch;
  int upper;
  int lower;
  int number;
  int special;

  if (!value || jvmud_size(value) < 8) {
    return "Password must be at least 8 characters.";
  }
  if (jvmud_size(value) > 72) {
    return "Password must be 72 characters or fewer.";
  }

  index = 0;
  while (index < jvmud_size(value)) {
    ch = value[index];
    if (ch >= 'A' && ch <= 'Z') {
      upper = 1;
    } else if (ch >= 'a' && ch <= 'z') {
      lower = 1;
    } else if (ch >= '0' && ch <= '9') {
      number = 1;
    } else if (password_special(ch)) {
      special = 1;
    } else {
      return "Password contains an unsupported character.";
    }
    index = index + 1;
  }
  if (!upper || !lower || !number || !special) {
    return "Password needs uppercase, lowercase, number, and special characters.";
  }
  return 0;
}

int password_special(int ch) {
  return ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%'
      || ch == '^' || ch == '&' || ch == '*' || ch == '_' || ch == '.'
      || ch == '?' || ch == '+' || ch == '-';
}
