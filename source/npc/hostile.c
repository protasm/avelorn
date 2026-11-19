string display_name;
string gender;
string details;
string *identities;
object *contributors;
int level;
int max_health;
int health;
int minimum_damage;
int maximum_damage;
int experience_reward;
int copper_reward;
string quest_defeat_tag;
nosave object home;
nosave int defeated;
nosave int engaged;
nosave int encounter_size;
nosave int swing;
nosave object threatening;
nosave int interrupted;
nosave mixed *variants;
nosave mapping encounter_variant;
int bonded;

void initialize(mixed first_load) {
  if (!display_name) {
    display_name = "hostile creature";
  }
  if (!gender) {
    gender = "non-binary";
  }
  if (!details) {
    details = "It watches you with open hostility.";
  }
  if (!identities) {
    identities = ({ "creature" });
  }
  if (!contributors) {
    contributors = ({ });
  }
  if (!quest_defeat_tag) {
    quest_defeat_tag = "";
  }
  jvmud_bind_entity_alias(jvmud_current_lpc_object(), "entity", "creature");
}

void configure(
    string name,
    string selected_gender,
    string description,
    int selected_level,
    int hit_points,
    int low_damage,
    int high_damage,
    int xp,
    int coins) {
  display_name = name;
  gender = selected_gender;
  details = description;
  level = selected_level;
  max_health = hit_points;
  health = hit_points;
  minimum_damage = low_damage;
  maximum_damage = high_damage;
  experience_reward = xp;
  copper_reward = coins;
  jvmud_bind_entity_alias(
      jvmud_current_lpc_object(),
      "entity",
      jvmud_lowercase_text(display_name));
}

void add_identity(string identity) {
  identities += ({ identity });
  jvmud_bind_entity_alias(jvmud_current_lpc_object(), "entity", identity);
}

void set_quest_defeat_tag(string tag) {
  quest_defeat_tag = tag;
}

void set_bonded(int enabled) {
  bonded = enabled;
}

void set_variants(mixed *choices) {
  variants = choices;
}

int query_hostile() {
  return !defeated;
}

string query_name() {
  return display_name;
}

string query_gender() {
  return gender;
}

int query_level() {
  return level;
}

int id(mixed value) {
  int index;
  string normalized;

  if (!value) {
    return 0;
  }
  normalized = jvmud_lowercase_text(value);
  if (normalized == jvmud_lowercase_text(display_name)) {
    return 1;
  }
  index = 0;
  while (index < jvmud_size(identities)) {
    if (normalized == identities[index]) {
      return 1;
    }
    index += 1;
  }
  return 0;
}

string short() {
  return display_name;
}

void describe(object viewer) {
  string subject_word;
  string be_word;
  mapping variant;

  subject_word = jvmud_invoke_lpc_object("system/pronouns", "subject", gender);
  be_word = jvmud_invoke_lpc_object("system/pronouns", "be_present", gender);
  write(display_name + "\n");
  write(details + "\n");
  write(jvmud_capitalize_text(subject_word) + " " + be_word + " a level " + level);
  write(" combatant with " + health + "/" + max_health + " health.\n");
  variant = active_variant();
  if (variant) {
    write(variant["name"] + ". " + variant["description"] + "\n");
  }
}

int receive_attack(object attacker, int damage) {
  object place;

  place = jvmud_entity_location(jvmud_current_lpc_object());
  if (defeated || !attacker || jvmud_entity_location(attacker) != place) {
    return 0;
  }
  if (!engaged) {
    start_encounter(attacker, place);
  }
  remember_contributor(attacker);
  if (bonded) {
    if (jvmud_invoke_lpc_object(attacker, "query_concord_weapon")) {
      avelorn_emit_at(place, "Concord metal makes the bonded armor go still.\n");
    } else {
      damage = damage * 3 / 4;
    }
  }
  if (damage < 1) { damage = 1; }
  health -= damage;
  avelorn_emit_at(
      place,
      jvmud_invoke_lpc_object(attacker, "query_name") + " strikes "
          + display_name + " for " + damage + " damage.\n");
  if (health <= 0) {
    defeat(place);
  }
  return 1;
}

int combat_health() {
  return max_health * (100 + (encounter_size - 1) * 55) / 100;
}

void start_encounter(object attacker, object place) {
  mixed *roster;
  int index;

  encounter_variant = active_variant();
  engaged = 1;
  encounter_size = 1;
  roster = jvmud_invoke_lpc_object("system/party", "roster", attacker);
  for (index = 0; index < jvmud_size(roster); index += 1) {
    if (roster[index] != attacker && jvmud_entity_location(roster[index]) == place) {
      encounter_size += 1;
    }
  }
  health = combat_health();
  swing = 0;
  warn_attack(attacker);
}

mapping active_variant() {
  int count;
  int week;

  if (engaged) { return encounter_variant; }
  count = jvmud_size(variants);
  if (!count) { return 0; }
  week = jvmud_invoke_lpc_object("system/quests", "current_rotation");
  return variants[week % count];
}

int heavy_swing() {
  int interval;

  interval = encounter_variant ? encounter_variant["heavy_every"] : 3;
  return swing % interval == 0;
}

void warn_attack(object target) {
  threatening = target;
  interrupted = 0;
  swing += 1;
  if (heavy_swing()) {
    avelorn_emit_at(jvmud_entity_location(jvmud_current_lpc_object()),
        display_name + " gathers for a heavy strike at "
            + jvmud_invoke_lpc_object(target, "query_name") + ".\n");
  } else {
    avelorn_emit_at(jvmud_entity_location(jvmud_current_lpc_object()),
        display_name + " closes on " + jvmud_invoke_lpc_object(target, "query_name") + ".\n");
  }
  jvmud_schedule_deferred_callback("strike", 1);
}

void interrupt_attack() {
  if (threatening && heavy_swing()) {
    interrupted = 1;
  }
}

void strike() {
  object target;
  object place;
  int damage;

  target = threatening;
  threatening = 0;
  place = jvmud_entity_location(jvmud_current_lpc_object());
  if (!engaged || defeated) {
    return;
  }
  if (active_fighter(target, place)) {
    if (interrupted) {
      avelorn_emit_at(place, display_name + " loses the opening.\n");
    } else {
      damage = minimum_damage + jvmud_random(maximum_damage - minimum_damage + 1);
      damage += encounter_size - 1;
      if (heavy_swing()) {
        damage += maximum_damage / 2;
        if (encounter_variant) { damage += encounter_variant["damage_bonus"]; }
      }
      jvmud_invoke_lpc_object(target, "receive_damage", damage, display_name);
    }
  }
  jvmud_schedule_deferred_callback("choose_target", 1);
}

int active_fighter(object fighter, object place) {
  return fighter && jvmud_interactive(fighter) && jvmud_entity_location(fighter) == place
      && jvmud_invoke_lpc_object(fighter, "query_combat_target") == jvmud_current_lpc_object();
}

void choose_target() {
  object place;
  object candidate;
  int size;
  int index;

  if (!engaged || defeated) {
    return;
  }
  place = jvmud_entity_location(jvmud_current_lpc_object());
  size = jvmud_size(contributors);
  for (index = 0; index < size; index += 1) {
    candidate = contributors[(swing + index) % size];
    if (active_fighter(candidate, place)) {
      warn_attack(candidate);
      return;
    }
  }
  engaged = 0;
  health = max_health;
  contributors = ({ });
}

void remember_contributor(object attacker) {
  int index;

  index = 0;
  while (index < jvmud_size(contributors)) {
    if (contributors[index] == attacker) {
      return;
    }
    index += 1;
  }
  contributors += ({ attacker });
}

void defeat(object place) {
  int index;
  object contributor;

  defeated = 1;
  engaged = 0;
  threatening = 0;
  jvmud_cancel_deferred_callback("strike");
  jvmud_cancel_deferred_callback("choose_target");
  home = place;
  avelorn_emit_at(place, display_name + " is defeated.\n");
  index = 0;
  while (index < jvmud_size(contributors)) {
    contributor = contributors[index];
    if (contributor && jvmud_entity_location(contributor) == place) {
      jvmud_invoke_lpc_object(
          contributor,
          "award_victory",
          experience_reward,
          copper_reward,
          display_name,
          jvmud_current_lpc_object());
      jvmud_invoke_lpc_object(
          contributor,
          "record_quest_defeat",
          quest_defeat_tag);
    }
    index += 1;
  }
  contributors = ({ });
  jvmud_move_entity(jvmud_current_lpc_object(), "place/offstage");
  jvmud_schedule_deferred_callback("respawn", 15);
}

void respawn() {
  health = max_health;
  defeated = 0;
  if (home) {
    jvmud_move_entity(jvmud_current_lpc_object(), home);
  }
}
