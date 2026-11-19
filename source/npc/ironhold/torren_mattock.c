inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Torren Mattock",
    "gender": "male",
    "role": "foreman of an unlicensed bound company",
    "description":
        "Torren talks about the families whose people were sent below the Empty Winch and " +
        "never called back up.",
    "aliases": ({ "torren", "foreman" }),
    "quest": "a-rope-held-fast"
  ]);
}
