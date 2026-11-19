inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Veska Lowe",
    "gender": "female",
    "role": "guest-house advocate",
    "description": "Veska hears the cases of families who lost their sponsor when the records were seized.",
    "aliases": ({ "veska", "advocate" }),
    "quest": "an-open-door"
  ]);
}
