inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Merev Call",
    "gender": "female",
    "role": "steward of the renewal assembly",
    "description": "Merev has written down exactly who agreed to escort the prisoners and who was made to.",
    "aliases": ({ "merev", "steward" }),
    "quest": "the-unforced-oath"
  ]);
}
