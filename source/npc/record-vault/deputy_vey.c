inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Deputy Vey",
    "gender": "female",
    "role": "enemy",
    "description":
        "The deputy has stacked confiscated seals close enough to a charcoal stove to melt if " +
        "the door falls. Her sword carries an official witness mark.",
    "aliases": ({ "vey", "deputy" }),
    "level": 4,
    "health": 58,
    "minimumDamage": 4,
    "maximumDamage": 7,
    "experience": 94,
    "copper": 24,
    "defeatTag": "seal-thief"
  ]);
}
