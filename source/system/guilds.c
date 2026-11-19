nosave mapping catalog;
nosave mapping providers;

void initialize(mixed first_load) {
  mixed registrations;
  mixed *ids;
  mixed entry;
  string id;
  string path;
  object provider;
  int index;

  registrations = jvmud_invoke_lpc_object("guild/registry", "registrations");
  if (!jvmud_is_mapping(registrations)) {
    jvmud_raise_error("The guild registry must return an ID-to-source mapping.");
  }
  catalog = ([ ]);
  providers = ([ ]);
  ids = jvmud_mapping_keys(registrations);
  for (index = 0; index < jvmud_size(ids); index += 1) {
    id = ids[index];
    path = registrations[id];
    if (!jvmud_size(id) || id != jvmud_lowercase_text(id)
        || !jvmud_is_string(path) || !jvmud_size(path)) {
      jvmud_raise_error("Invalid guild registration for " + id + ".");
    }
    provider = jvmud_load_lpc_object(path);
    entry = jvmud_invoke_lpc_object(provider, "definition");
    if (!jvmud_is_mapping(entry) || !jvmud_is_string(entry["name"]) || !jvmud_size(entry["name"])
        || !jvmud_is_string(entry["description"]) || !jvmud_size(entry["description"])) {
      jvmud_raise_error("Guild " + id + " needs a name and description in " + path + ".");
    }
    catalog[id] = entry;
    providers[id] = provider;
  }
}

mixed definition(string id) {
  return catalog[id];
}

mixed *ids() {
  return jvmud_mapping_keys(catalog);
}

int known(string id) {
  return jvmud_member(catalog, id);
}

int eligible(string id, object persona) {
  object provider;

  provider = providers[id];
  if (!provider || !persona) {
    return 0;
  }
  if (!jvmud_method_exists("eligible", provider)) {
    return 1;
  }
  return jvmud_invoke_lpc_object(provider, "eligible", persona) != 0;
}
