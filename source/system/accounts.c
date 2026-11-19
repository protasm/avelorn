nosave mapping owners;

void initialize(mixed first_load) {
  if (!owners) {
    owners = ([ ]);
  }
}

int claim(string account, object player) {
  if (owners[account] && owners[account] != player) {
    return 0;
  }
  owners[account] = player;
  return 1;
}

void release(string account, object player) {
  if (owners[account] == player) {
    owners = jvmud_mapping_delete(owners, account);
  }
}

int exists(string stem) {
  return jvmud_read_mudlib_text(stem + ".o")
      || jvmud_size(jvmud_list_mudlib_paths(stem + ".o")) > 0;
}

int checkpoint_generation(string stem, string account) {
  mixed document;
  mapping fields;
  mixed *keys;
  string key;
  string prefix;
  mixed character_class;
  int length;
  int index;
  int generation;

  if (catch(document = jvmud_read_mudlib_json(stem + ".o"))) {
    return 0;
  }
  if (!jvmud_is_mapping(document) || document["format"] != "jvmud.lpc-object-state"
      || document["version"] != 1 || !jvmud_is_mapping(document["fields"])) {
    return 0;
  }
  fields = document["fields"];
  keys = jvmud_mapping_keys(fields);
  for (index = 0; index < jvmud_size(keys); index += 1) {
    key = keys[index];
    length = jvmud_size(key);
    if (length > 20 && jvmud_extract_text(key, length - 20, length - 1)
        == ".snapshot_generation") {
      prefix = jvmud_extract_text(key, 0, length - 20);
      character_class = field_value(fields, prefix + "character_class");
      if (field_value(fields, prefix + "account_id") != account
          || field_value(fields, prefix + "account_created") != 1
          || field_value(fields, prefix + "save_version") != 2
          || !valid_text_field(fields, prefix + "password_hash")
          || !valid_text_field(fields, prefix + "character_name")
          || !jvmud_is_string(character_class)
          || !jvmud_invoke_lpc_object("system/combat_rules", "known", character_class)) {
        return 0;
      }
      if (!jvmud_is_int(field_value(fields, key))) {
        return 0;
      }
      generation = field_value(fields, key);
      return generation > 0 ? generation : 0;
    }
  }
  return 0;
}

mixed field_value(mapping fields, string key) {
  mixed field;

  field = fields[key];
  return jvmud_is_mapping(field) ? field["value"] : 0;
}

int valid_text_field(mapping fields, string key) {
  mixed value;

  value = field_value(fields, key);
  return jvmud_is_string(value) && jvmud_size(value) > 0;
}
