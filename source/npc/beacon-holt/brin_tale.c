inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Brin Tale",
    "gender": "male",
    "role": "second keeper of the northern beacons",
    "description": "Brin wants a witness who will acknowledge what the officer's log omitted.",
    "aliases": ({ "brin", "keeper" }),
    "quest": "another-light"
  ]);
}
