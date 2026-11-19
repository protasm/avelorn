inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Edran Sell",
    "gender": "male",
    "role": "registrar of the Open Roof Hall",
    "description": "Edran wants to recover the names of guests whose shelter debts were sold to the Dominion.",
    "aliases": ({ "edran", "registrar" }),
    "quest": "the-guests-roll"
  ]);
}
