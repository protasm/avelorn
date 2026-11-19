inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "headland wolf",
    "gender": "female",
    "role": "enemy",
    "description": "Salt mats the wolf's coat. It has learned that the fish baskets come up the lamp walk every evening.",
    "aliases": ({ "wolf", "headland wolf" }),
    "level": 1,
    "health": 30,
    "minimumDamage": 2,
    "maximumDamage": 4,
    "experience": 18,
    "copper": 5,
    "defeatTag": "headland-wolf"
  ]);
}
