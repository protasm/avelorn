inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Senna Blade",
    "gender": "female",
    "role": "unlicensed witness",
    "description": "Senna remembers the testimony of a family the official register says never existed.",
    "aliases": ({ "senna", "witness" }),
    "quest": "a-witness-for-the-lost"
  ]);
}
