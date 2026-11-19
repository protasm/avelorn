string blueprint_id;
string display_name;
string keyword;
string details;
string equipment_slot;
string governing_stat;
int weight;
int copper_value;
int recommended_level;
int minimum_stat;
int restore_amount;
int power;
int protection;
string damage_type;
int concord;
int travel_restore;
int equipped;

mapping item_profile() { return ([ ]); }

void initialize(mixed first_load) {
  mapping profile;

  profile = item_profile();
  if (jvmud_size(profile) > 0) {
    configure(query_blueprint(), profile["name"], profile["alias"], profile["description"],
        profile["slot"], profile["weight"], profile["value"], profile["level"], profile["stat"],
        profile["minimum"], profile["healing"]);
    set_profile(profile);
    return;
  }
  if (!blueprint_id) {
    blueprint_id = "unknown-item";
  }
  if (!display_name) {
    display_name = "unremarkable item";
  }
  if (!keyword) {
    keyword = "item";
  }
  if (!details) {
    details = "It has not yet been described.";
  }
  if (!equipment_slot) {
    equipment_slot = "none";
  }
  if (!governing_stat) {
    governing_stat = "none";
  }
}

void configure(
    string id,
    string name,
    string identity,
    string description,
    string slot,
    int item_weight,
    int value,
    int suggested_level,
    string required_stat,
    int suggested_stat,
    int healing) {
  blueprint_id = id;
  display_name = name;
  keyword = identity;
  details = description;
  equipment_slot = slot;
  weight = item_weight;
  copper_value = value;
  recommended_level = suggested_level;
  governing_stat = required_stat;
  minimum_stat = suggested_stat;
  restore_amount = healing;
  jvmud_bind_entity_alias(jvmud_current_lpc_object(), "entity", keyword);
  jvmud_bind_entity_alias(jvmud_current_lpc_object(), "entity", jvmud_lowercase_text(display_name));
  jvmud_bind_entity_alias(
      jvmud_current_lpc_object(),
      "blueprint",
      blueprint_id);
}

void set_profile(mapping profile) {
  power = profile["power"];
  protection = profile["protection"];
  damage_type = profile["damage_type"];
  concord = profile["concord"];
  travel_restore = profile["travel_restore"];
}

int id(mixed value) {
  string normalized;

  if (!value) {
    return 0;
  }
  normalized = jvmud_lowercase_text(value);
  return normalized == keyword
      || normalized == jvmud_lowercase_text(display_name)
      || normalized == blueprint_id;
}

string short() {
  if (equipped) {
    return display_name + " (equipped)";
  }
  return display_name;
}

void describe(object viewer) {
  write(display_name + "\n");
  write(details + "\n");
  write("Weight: " + weight + ". Value: ");
  write(jvmud_invoke_lpc_object("system/economy", "format_money", copper_value) + ".\n");
  if (power > 0) {
    write("Weapon power: " + power + " (" + damage_type + ").\n");
  }
  if (protection > 0) {
    write("Armor protection: " + protection + ".\n");
  }
  if (concord) {
    write("Concord metal can briefly interrupt a bond.\n");
  }
  if (restore_amount > 0) {
    write("Restores up to " + restore_amount + " health.\n");
  }
  if (travel_restore > 0) {
    write("Restores up to " + travel_restore + " travel reserve.\n");
  }
  if (recommended_level > 1) {
    write("Recommended level: " + recommended_level + ".\n");
  }
  if (minimum_stat > 0) {
    write("Recommended " + governing_stat + ": " + minimum_stat + ".\n");
  }
}

int can_take(object actor) {
  return 1;
}

// Item programs may override this hook. Zero declines use, one retains the item,
// and two lets the caller consume it after the effect runs.
int use_by(object actor) {
  int reserve;

  if (!actor || (restore_amount <= 0 && travel_restore <= 0)) {
    return 0;
  }
  avelorn_write_to(actor, "You use " + display_name + ".\n");
  if (restore_amount > 0) {
    jvmud_invoke_lpc_object(actor, "receive_healing", restore_amount);
  }
  if (travel_restore > 0) {
    reserve = jvmud_invoke_lpc_object(actor, "adjust_travel_reserve", travel_restore);
    avelorn_write_to(actor, "Travel reserve " + reserve + "/100.\n");
  }
  return 2;
}

string query_blueprint() {
  return blueprint_id;
}

string query_slot() {
  return equipment_slot;
}

string query_governing_stat() {
  return governing_stat;
}

int query_weight() {
  return weight;
}

int query_value() {
  return copper_value;
}

int query_recommended_level() {
  return recommended_level;
}

int query_minimum_stat() {
  return minimum_stat;
}

int query_restore_amount() {
  return restore_amount;
}

int query_power() { return power; }
int query_protection() { return protection; }
string query_damage_type() { return damage_type; }
int query_concord() { return concord; }
int query_travel_restore() { return travel_restore; }

int query_equipped() {
  return equipped;
}

void set_equipped(int value) {
  equipped = value ? 1 : 0;
}
