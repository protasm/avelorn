inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Sella Fane",
    "gender": "female",
    "role": "foster-house steward",
    "description": "Sella shares a roof with families fleeing both the Dominion and their sponsors' household contracts.",
    "aliases": ({ "sella", "steward" }),
    "quest": "room-at-the-table"
  ]);
}
