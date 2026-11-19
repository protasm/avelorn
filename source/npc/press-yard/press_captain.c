inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Press Captain",
    "gender": "female",
    "role": "enemy",
    "description": "Her shield carries a tally mark for each sailor she considers still bound to her.",
    "aliases": ({ "captain" }),
    "level": 10,
    "health": 145,
    "minimumDamage": 6,
    "maximumDamage": 11,
    "experience": 150,
    "copper": 42,
    "variants": ({
      ([
        "name": "Press Captain of the Hook Line",
        "description":
            "The captain turns her old tally shield sideways and hooks the gap with a "
            + "short boarding weapon. She keeps close enough to strike again before there "
            + "is room to back away.",
        "heavy_every": 2,
        "damage_bonus": 0
      ]),
      ([
        "name": "Press Captain of the Levy Roll",
        "description":
            "A heavy blade hangs from the red cord threaded through her tally shield. She "
            + "presses the shield against the wall and waits for a clean cut across the "
            + "lane.",
        "heavy_every": 3,
        "damage_bonus": 2
      ])
    }),
    "defeatTag": "press-captain"
  ]);
}
