inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Mold Sentinel",
    "gender": "male",
    "role": "enemy",
    "description":
        "A furnace guard with bonded plates joined across his wrists. He watches the empty " +
        "fire door for his shift relief.",
    "aliases": ({ "sentinel", "mold" }),
    "level": 10,
    "health": 155,
    "minimumDamage": 6,
    "maximumDamage": 11,
    "experience": 150,
    "copper": 42,
    "variants": ({
      ([
        "name": "Mold Sentinel of the Cooling Floor",
        "description":
            "Cold inlets hold the sentinel's armor tight. He has learned to pivot at the "
            + "wrists and take fast cuts across the rows of brittle clay.",
        "heavy_every": 2,
        "damage_bonus": 1
      ]),
      ([
        "name": "Mold Sentinel of the Furnace Door",
        "description":
            "Heat lets the sentinel's bonded plates slide freely against one another. He "
            + "lets them gather behind a single blow from the heel of his iron tool.",
        "heavy_every": 4,
        "damage_bonus": 3
      ])
    }),
    "defeatTag": "mold-sentinel"
  ]);
}
