inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Bond Broker Harl",
    "gender": "male",
    "role": "enemy",
    "description":
        "Harl wears a cudgel on a sash threaded through two seals, one a sponsor family's and " +
        "the other the Dominion tax office's.",
    "aliases": ({ "harl", "broker" }),
    "level": 3,
    "health": 47,
    "minimumDamage": 3,
    "maximumDamage": 6,
    "experience": 65,
    "copper": 18,
    "defeatTag": "bond-broker"
  ]);
}
