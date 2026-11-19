inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Overseer Carrow",
    "gender": "male",
    "role": "enemy",
    "description":
        "Carrow's mail tightens with every movement of his shoulders. His company rope has " +
        "been braided into a single continuous ring.",
    "aliases": ({ "carrow", "overseer" }),
    "level": 8,
    "health": 105,
    "minimumDamage": 5,
    "maximumDamage": 9,
    "experience": 190,
    "copper": 45,
    "defeatTag": "bonded-overseer",
    "bonded": 1
  ]);
}
