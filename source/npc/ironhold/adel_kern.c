inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Adel Kern",
    "gender": "female",
    "role": "old company factor",
    "description": "Adel has a debt list deliberately ordered by unmet wages rather than quantity of ore.",
    "aliases": ({ "adel", "factor" }),
    "quest": "shared-iron"
  ]);
}
