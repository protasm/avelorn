inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Iona Venn",
    "gender": "female",
    "role": "Westwatch guest-house keeper",
    "description": "Her second register lists guests who left without naming a destination.",
    "aliases": ({ "iona", "keeper" }),
    "quest": "lamp-on-the-headland"
  ]);
}
