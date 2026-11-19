nosave mapping paths;

void initialize(mixed first_load) {
  paths = ([
    "iona-venn": "npc/westwatch/iona_venn",
    "cobin-holl": "npc/westwatch/cobin_holl",
    "rhea-kett": "npc/westwatch/rhea_kett",
    "headland-wolf": "npc/headland-signal/headland_wolf",
    "edran-sell": "npc/havenrode/edran_sell",
    "senna-blade": "npc/witnessgate/senna_blade",
    "aften-vale": "npc/keepingstead/aften_vale",
    "merev-call": "npc/freeharbor/merev_call",
    "torren-mattock": "npc/ironhold/torren_mattock",
    "bond-broker": "npc/salt-ledger/bond_broker_harl",
    "seal-thief": "npc/record-vault/deputy_vey",
    "ration-sergeant": "npc/silt-house/ration_sergeant_fenn",
    "oath-bailiff": "npc/tide-chapel/bailiff_carn",
    "bonded-overseer": "npc/empty-winch/overseer_carrow",
    "sella-fane": "npc/greenhame/sella_fane",
    "brin-tale": "npc/beacon-holt/brin_tale",
    "lena-sot": "npc/orchard-cross/lena_sot",
    "tamsin-rowe": "npc/driftwood/tamsin_rowe",
    "bertan-lent": "npc/ropebridge/bertan_lent",
    "avren-toll": "npc/coalstair/avren_toll",
    "bren-rusk": "npc/cinder-bank/bren_rusk",
    "elle-hearth": "npc/cinder-bank/elle_hearth",
    "hevern-ink": "npc/cinder-bank/hevern_ink",
    "eris-reed": "npc/cinder-bank/eris_reed",
    "maren-shore": "npc/cinder-bank/maren_shore",
    "vault-warden": "npc/deep-vault/warden_senk",
    "ledger-enforcer": "npc/bond-yard/ledger_enforcer",
    "veska-lowe": "npc/havenrode/veska_lowe",
    "inkback-heron": "npc/drowned-archive/inkback_heron",
    "callen-lint": "npc/witnessgate/callen_lint",
    "kilnback-boar": "npc/red-run/kilnback_boar",
    "nara-crook": "npc/keepingstead/nara_crook",
    "press-captain": "npc/press-yard/press_captain",
    "pol-eld": "npc/freeharbor/pol_eld",
    "mold-sentinel": "npc/old-smelter/mold_sentinel",
    "adel-kern": "npc/ironhold/adel_kern"
  ]);
}

mixed *ids() {
  return jvmud_mapping_keys(paths);
}

mixed definition(string id) {
  string path;

  path = paths[id];
  return jvmud_size(path) ? jvmud_invoke_lpc_object(path, "definition") : 0;
}

object create(string id) {
  string path;
  object entity;
  mapping entry;
  mixed *aliases;
  int index;

  path = paths[id];
  if (!jvmud_size(path)) {
    return 0;
  }
  entity = jvmud_clone_lpc_object(path);
  entry = jvmud_invoke_lpc_object(entity, "definition");
  if (entry["kind"] == "hostile") {
    jvmud_invoke_lpc_object(entity, "configure", entry["name"], entry["gender"],
        entry["description"], entry["level"], entry["health"], entry["minimumDamage"],
        entry["maximumDamage"], entry["experience"], entry["copper"]);
    if (jvmud_size(entry["defeatTag"])) {
      jvmud_invoke_lpc_object(entity, "set_quest_defeat_tag", entry["defeatTag"]);
    }
    if (entry["bonded"]) {
      jvmud_invoke_lpc_object(entity, "set_bonded", 1);
    }
    if (jvmud_size(entry["variants"])) {
      jvmud_invoke_lpc_object(entity, "set_variants", entry["variants"]);
    }
  } else if (entry["kind"] == "quest_giver") {
    jvmud_invoke_lpc_object(entity, "configure", entry["name"], entry["gender"],
        entry["role"], entry["quest"]);
    jvmud_invoke_lpc_object(entity, "set_details", entry["description"]);
  } else {
    jvmud_invoke_lpc_object(entity, "configure", entry["name"], entry["gender"],
        entry["role"], entry["description"]);
    if (jvmud_size(entry["quest"])) {
      jvmud_invoke_lpc_object(entity, "set_quest", entry["quest"]);
    }
  }
  aliases = entry["aliases"];
  for (index = 0; index < jvmud_size(aliases); index += 1) {
    jvmud_invoke_lpc_object(entity, "add_identity", aliases[index]);
  }
  return entity;
}
