inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "inkback heron",
    "gender": "female",
    "role": "enemy",
    "description":
        "Black mineral beads harden the bird's pinions, and each movement fans dirty water " +
        "across the high shelves.",
    "aliases": ({ "heron", "inkback" }),
    "level": 10,
    "health": 130,
    "minimumDamage": 5,
    "maximumDamage": 11,
    "experience": 150,
    "copper": 42,
    "variants": ({
      ([
        "name": "Low-water inkback heron",
        "description":
            "The bird keeps its feet on a row of exposed register racks. With the water "
            + "low, she can jab again before her wings have settled.",
        "heavy_every": 2,
        "damage_bonus": 0
      ]),
      ([
        "name": "Spate inkback heron",
        "description":
            "The heron waits under cover of the flooded shelf. When she rises, the stiff "
            + "black tips of both wings sweep water and loose testimony into the attack.",
        "heavy_every": 4,
        "damage_bonus": 3
      ])
    }),
    "defeatTag": "inkback-heron"
  ]);
}
