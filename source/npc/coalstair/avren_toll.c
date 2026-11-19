inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Avren Toll",
    "gender": "female",
    "role": "guide of the highest bound company",
    "description":
        "Avren knows the last winding below the mountain. She waits for someone carrying " +
        "concord before asking for help.",
    "aliases": ({ "avren", "guide" }),
    "quest": "under-the-mountain"
  ]);
}
