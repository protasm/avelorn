inherit "npc/quest_giver";

mapping definition() {
  return ([
    "kind": "quest_giver",
    "name": "Bertan Lent",
    "gender": "male",
    "role": "bridge runner of the old company",
    "description": "Bertan still counts every strand of the unmarked rope and every person who places a hand on it.",
    "aliases": ({ "bertan", "runner" }),
    "quest": "a-hand-on-the-rope"
  ]);
}
