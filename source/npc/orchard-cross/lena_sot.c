inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Lena Sot",
    "gender": "female",
    "role": "keeper of the common orchard shares",
    "description":
        "Lena believes someone's repair ledger left the southern plots out of a crucial annual " +
        "drain clearing.",
    "aliases": ({ "lena", "keeper" }),
    "quest": "the-missing-graft"
  ]);
}
