nosave mapping catalog;
nosave mapping defeats;
nosave mapping actions;

void initialize(mixed first_load) {
  mapping paths;
  mixed *ids;
  string id;
  mapping quest;
  mixed *tags;
  int index;
  int tag_index;

  paths = ([
    "lamp-on-the-headland": "quest/westwatch/lamp_on_the_headland",
    "the-guests-roll": "quest/havenrode/the_guests_roll",
    "a-witness-for-the-lost": "quest/witnessgate/a_witness_for_the_lost",
    "a-storehouse-in-trust": "quest/keepingstead/a_storehouse_in_trust",
    "the-unforced-oath": "quest/freeharbor/the_unforced_oath",
    "a-rope-held-fast": "quest/ironhold/a_rope_held_fast",
    "room-at-the-table": "quest/greenhame/room_at_the_table",
    "another-light": "quest/beacon-holt/another_light",
    "the-missing-graft": "quest/orchard-cross/the_missing_graft",
    "the-right-of-salvage": "quest/driftwood/the_right_of_salvage",
    "a-hand-on-the-rope": "quest/ropebridge/a_hand_on_the_rope",
    "the-concordance": "quest/cinder-bank/the_concordance",
    "under-the-mountain": "quest/coalstair/under_the_mountain",
    "an-open-door": "quest/havenrode/an_open_door",
    "dry-the-record": "quest/witnessgate/dry_the_record",
    "hold-the-red-run": "quest/keepingstead/hold_the_red_run",
    "we-owe-no-oath": "quest/freeharbor/we_owe_no_oath",
    "shared-iron": "quest/ironhold/shared_iron",
    "three-shared-bowls": "quest/westwatch/three_shared_bowls",
    "hoops-for-the-landing": "quest/westwatch/hoops_for_the_landing",
    "a-nail-without-a-name": "quest/havenrode/a_nail_without_a_name",
    "an-empty-bunk": "quest/greenhame/an_empty_bunk",
    "salt-before-sun": "quest/saltmere/salt_before_sun",
    "tarbills": "quest/havenrode/tarbills",
    "the-uncut-seal": "quest/witnessgate/the_uncut_seal",
    "two-points-of-ink": "quest/witnessgate/two_points_of_ink",
    "when-the-causeway-fell": "quest/reedford/when_the_causeway_fell",
    "the-whitequay-weighing": "quest/whitequay/the_whitequay_weighing",
    "before-bell-one": "quest/beacon-holt/before_bell_one",
    "dry-sacks": "quest/keepingstead/dry_sacks",
    "the-far-wall": "quest/saltbreak/the_far_wall",
    "the-older-repair-ledger": "quest/eastweir/the_older_repair_ledger",
    "giving-a-share-back": "quest/orchard-cross/giving_a_share_back",
    "weir-keys": "quest/keepingstead/weir_keys",
    "the-chair-by-the-door": "quest/freeharbor/the_chair_by_the_door",
    "a-charter-in-scraps": "quest/driftwood/a_charter_in_scraps",
    "fifth-crew-refusal": "quest/grey-step/fifth_crew_refusal",
    "the-third-mooring": "quest/freeharbor/the_third_mooring",
    "pay-the-drover": "quest/moorcross/pay_the_drover",
    "someone-holds-the-back": "quest/ropebridge/someone_holds_the_back",
    "company-wages": "quest/ironhold/company_wages",
    "under-an-old-wage": "quest/coalstair/under_an_old_wage",
    "terms-at-the-shelf": "quest/lakeward/terms_at_the_shelf",
    "three-voices-at-cinder-bank": "quest/cinder-bank/three_voices_at_cinder_bank",
    "a-light-below-the-winch": "quest/ironhold/a_light_below_the_winch",
    "shells-beneath-the-quay": "quest/havenrode/shells_beneath_the_quay",
    "the-lowest-flood-mark": "quest/witnessgate/the_lowest_flood_mark",
    "first-water-round": "quest/keepingstead/first_water_round",
    "the-voluntary-watch": "quest/freeharbor/the_voluntary_watch",
    "the-companions-count": "quest/ironhold/the_companions_count"
  ]);
  catalog = ([ ]);
  defeats = ([ ]);
  actions = ([ ]);
  ids = jvmud_mapping_keys(paths);
  for (index = 0; index < jvmud_size(ids); index += 1) {
    id = ids[index];
    quest = jvmud_invoke_lpc_object(paths[id], "definition");
    catalog[id] = quest;
    if (jvmud_size(quest["defeat_tag"])) {
      defeats[quest["defeat_tag"]] = id;
    }
    if (jvmud_size(quest["action_tag"])) {
      actions[quest["action_tag"]] = id;
    }
    tags = quest["defeat_tags"];
    for (tag_index = 0; tag_index < jvmud_size(tags); tag_index += 1) {
      defeats[tags[tag_index]] = id;
    }
    tags = quest["action_tags"];
    for (tag_index = 0; tag_index < jvmud_size(tags); tag_index += 1) {
      actions[tags[tag_index]] = id;
    }
  }
}

mixed *ids() {
  return jvmud_mapping_keys(catalog);
}

mixed definition(string id) {
  return catalog[id];
}

mixed field(string id, string name) {
  mapping quest;

  quest = catalog[id];
  return quest ? quest[name] : 0;
}

int known(string id) { return catalog[id] != 0; }
string title(string id) { return known(id) ? field(id, "title") : "Unknown assignment"; }
string description(string id) { return field(id, "description"); }
int recommended_level(string id) { return field(id, "level"); }
int required_count(string id) { return field(id, "count"); }
int experience_reward(string id) { return field(id, "xp"); }
int copper_reward(string id) { return field(id, "copper"); }
string quest_for_defeat_tag(string tag) { return defeats[tag]; }
string quest_for_action_tag(string tag) { return actions[tag]; }
string item_reward(string id) { return field(id, "item"); }
string reward_for_class(string id, string character_class) {
  mixed rewards;

  rewards = field(id, "class_rewards");
  return rewards && rewards[character_class] ? rewards[character_class] : item_reward(id);
}
string prerequisite(string id) { return field(id, "prerequisite"); }
int repeatable(string id) { return field(id, "repeatable"); }
int repeatable_defeat_tag(string tag) {
  string id;

  id = defeats[tag];
  return repeatable(id) && !jvmud_size(field(id, "defeat_tags"));
}

int current_rotation() {
  return jvmud_time() / 604800;
}
