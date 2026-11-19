inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Bren Rusk",
    "gender": "male",
    "role": "Durnathi dissident and former forge steward",
    "description":
        "Bren walked out on the foundry that paid his family. He now checks that everyone who " +
        "takes a place by the fire may leave it.",
    "aliases": ({ "bren", "steward" }),
    "quest": "the-concordance"
  ]);
}
