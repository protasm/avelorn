nosave mixed *blueprint_ids;
nosave mapping markets;

void initialize(mixed first_load) {
  blueprint_ids = ({
    "weapon/arming-sword",
    "weapon/ashwood-shortbow",
    "weapon/oak-focus-staff",
    "weapon/steel-mace",
    "weapon/netters-knife",
    "weapon/yard-cudgel",
    "weapon/havenrode-sabre",
    "weapon/freeharbor-spear",
    "weapon/ironhold-maul",
    "weapon/witness-recurve",
    "weapon/keeping-longbow",
    "weapon/ironhold-crossbow",
    "weapon/beacon-focus",
    "weapon/witness-focus",
    "weapon/lakeward-staff",
    "weapon/keepers-hammer",
    "weapon/assembly-mace",
    "weapon/ropeward-hammer",
    "weapon/concord-sword",
    "weapon/concord-bow",
    "weapon/concord-focus",
    "weapon/concord-mace",
    "armor/travel-cloak",
    "armor/quilted-jack",
    "armor/shore-leathers",
    "armor/hearing-coat",
    "armor/keeping-brigandine",
    "armor/chain-hauberk",
    "armor/company-plate",
    "armor/roadward-duster",
    "armor/felt-cap",
    "armor/witness-hood",
    "armor/saltbreak-helm",
    "armor/ropeward-helmet",
    "armor/tallow-wraps",
    "armor/keeper-bracers",
    "consumable/healing-draught",
    "consumable/field-bread",
    "consumable/cistern-water",
    "consumable/wound-wrap",
    "consumable/salt-fish",
    "consumable/orchard-cordial",
    "consumable/trail-bundle",
    "consumable/boiler-tea",
    "consumable/keeping-salve",
    "badge/open-roof-pin",
    "badge/double-seal",
    "badge/red-run-knot",
    "badge/free-crew-braid",
    "badge/shared-iron-rivet"
  });
  markets = ([
    "westwatch": ({
      "consumable/field-bread",
      "consumable/cistern-water",
      "consumable/healing-draught",
      "weapon/netters-knife",
      "weapon/yard-cudgel",
      "armor/quilted-jack",
      "armor/felt-cap",
      "armor/tallow-wraps",
      "consumable/wound-wrap"
    }),
    "arvenna": ({
      "consumable/field-bread",
      "consumable/salt-fish",
      "consumable/healing-draught",
      "weapon/havenrode-sabre",
      "weapon/beacon-focus",
      "weapon/ashwood-shortbow",
      "armor/shore-leathers",
      "armor/quilted-jack",
      "armor/tallow-wraps",
      "consumable/trail-bundle"
    }),
    "ordel": ({
      "consumable/field-bread",
      "consumable/wound-wrap",
      "consumable/healing-draught",
      "weapon/witness-recurve",
      "weapon/witness-focus",
      "armor/hearing-coat",
      "armor/witness-hood",
      "consumable/cistern-water",
      "consumable/trail-bundle"
    }),
    "talvaren": ({
      "consumable/field-bread",
      "consumable/orchard-cordial",
      "consumable/healing-draught",
      "weapon/keeping-longbow",
      "weapon/keepers-hammer",
      "armor/keeping-brigandine",
      "armor/keeper-bracers",
      "armor/roadward-duster",
      "consumable/keeping-salve",
      "consumable/trail-bundle"
    }),
    "caereth": ({
      "consumable/field-bread",
      "consumable/salt-fish",
      "consumable/healing-draught",
      "weapon/freeharbor-spear",
      "weapon/assembly-mace",
      "armor/chain-hauberk",
      "armor/saltbreak-helm",
      "armor/shore-leathers",
      "consumable/trail-bundle"
    }),
    "durnath": ({
      "consumable/field-bread",
      "consumable/boiler-tea",
      "consumable/healing-draught",
      "weapon/ironhold-maul",
      "weapon/ironhold-crossbow",
      "weapon/lakeward-staff",
      "weapon/ropeward-hammer",
      "armor/company-plate",
      "armor/ropeward-helmet",
      "consumable/trail-bundle"
    })
  ]);
}

string canonical_id(string blueprint_id) {
  // Preserve equipment created before the geographic reset.
  if (blueprint_id == "weapon/crown-arming-sword") { return "weapon/arming-sword"; }
  if (blueprint_id == "weapon/temple-mace") { return "weapon/steel-mace"; }
  return blueprint_id;
}

int known(string blueprint_id) {
  return jvmud_member(blueprint_ids, canonical_id(blueprint_id)) >= 0;
}

mixed blueprint(string blueprint_id) {
  if (!known(blueprint_id)) { return 0; }
  return jvmud_invoke_lpc_object("item/" + canonical_id(blueprint_id), "item_profile");
}

mixed *ids() {
  return blueprint_ids;
}

mixed *stock(string area, string region) {
  mixed wares;

  wares = markets[area];
  if (jvmud_is_array(wares)) { return wares; }
  wares = markets[region];
  return jvmud_is_array(wares) ? wares : markets["westwatch"];
}

object create(string blueprint_id) {
  if (!known(blueprint_id)) { return 0; }
  return jvmud_clone_lpc_object("item/" + canonical_id(blueprint_id));
}
