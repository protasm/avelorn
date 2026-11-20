inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Iona Venn",
    "gender": "female",
    "role": "Westwatch guest-house keeper",
    "description": "Iona keeps a second register of guests who leave without saying where they are going.",
    "aliases": ({ "iona", "keeper" }),
    "quest": "lamp-on-the-headland"
  ]);
}
