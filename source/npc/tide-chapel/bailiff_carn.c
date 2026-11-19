inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Bailiff Carn",
    "gender": "female",
    "role": "enemy",
    "description":
        "Carn has recorded every prisoner's compelled journey as a voluntary pledge. Her spear " +
        "bears a sliver of the dark mountain substance.",
    "aliases": ({ "carn", "bailiff" }),
    "level": 6,
    "health": 80,
    "minimumDamage": 5,
    "maximumDamage": 8,
    "experience": 150,
    "copper": 34,
    "defeatTag": "oath-bailiff"
  ]);
}
