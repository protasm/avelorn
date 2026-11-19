nosave mixed *class_order;
nosave mapping profiles;

void initialize(mixed first_load) {
  string id;
  mixed entry;
  int index;

  class_order = jvmud_invoke_lpc_object("class/registry", "classes");
  profiles = ([ ]);
  for (index = 0; index < jvmud_size(class_order); index += 1) {
    id = class_order[index];
    entry = jvmud_invoke_lpc_object("class/" + id, "definition");
    if (!jvmud_is_mapping(entry) || entry["id"] != id || profiles[id]) {
      jvmud_raise_error("Invalid or duplicate class definition " + id + ".");
    }
    profiles[id] = entry;
  }
}

mixed *classes() {
  return class_order + ({ });
}

mixed profile(string id) {
  return profiles[id];
}

int known(string id) {
  return jvmud_is_mapping(profile(id));
}

mapping entries(string character_class, string category) {
  mixed rules;

  rules = profile(character_class);
  return rules ? rules[category] : ([ ]);
}

mixed *styles(string character_class) {
  mixed rules;

  rules = profile(character_class);
  return rules ? rules["style_order"] : ({ });
}

mixed *techniques(string character_class) {
  mixed rules;

  rules = profile(character_class);
  return rules ? rules["technique_order"] : ({ });
}

mixed style(string character_class, string name) {
  return entries(character_class, "styles")[name];
}

mixed technique(string character_class, string name) {
  return entries(character_class, "techniques")[name];
}
