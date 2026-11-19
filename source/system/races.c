nosave mapping catalog;
nosave string default_id;

void initialize(mixed first_load) {
  mapping registrations;
  mapping entry;
  mixed *keys;
  string id;
  int index;

  catalog = ([ ]);
  registrations = jvmud_invoke_lpc_object("race/registry", "registrations");
  keys = jvmud_mapping_keys(registrations);
  for (index = 0; index < jvmud_size(keys); index += 1) {
    id = keys[index];
    entry = jvmud_invoke_lpc_object(registrations[id], "definition");
    if (!jvmud_is_mapping(entry) || !jvmud_size(entry["name"])) {
      jvmud_raise_error("Missing race definition or name for " + id + ".");
    }
    catalog[id] = entry;
  }
  default_id = jvmud_invoke_lpc_object("race/registry", "default_race");
  if (!known(default_id)) {
    jvmud_raise_error("Unknown default race " + default_id + ".");
  }
}

string default_race() { return default_id; }
mixed *ids() { return jvmud_mapping_keys(catalog); }
mixed definition(string id) { return catalog[id]; }
int known(string id) { return definition(id) != 0; }
