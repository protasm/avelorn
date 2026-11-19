inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Tamsin Rowe",
    "gender": "female",
    "role": "salvage assessor",
    "description": "Tamsin knows the foundry tried to claim timber a fishing crew raised at its own expense.",
    "aliases": ({ "tamsin", "assessor" }),
    "quest": "the-right-of-salvage"
  ]);
}
