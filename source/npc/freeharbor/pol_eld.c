inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Pol Eld",
    "gender": "male",
    "role": "commission assessor",
    "description": "Pol records the value of recovered boats without assigning any crew a share they did not accept.",
    "aliases": ({ "pol", "assessor" }),
    "quest": "we-owe-no-oath"
  ]);
}
