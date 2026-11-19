inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "kilnback boar",
    "gender": "male",
    "role": "enemy",
    "description":
        "A heavy boar brushes the brickwork. Rust-colored growths have fused black waste from " +
        "the kiln into its hide.",
    "aliases": ({ "boar", "kilnback" }),
    "level": 10,
    "health": 140,
    "minimumDamage": 6,
    "maximumDamage": 10,
    "experience": 150,
    "copper": 42,
    "variants": ({
      ([
        "name": "Spillway kilnback boar",
        "description":
            "The boar uses the angled tiles to turn quickly after a brush with the wall. "
            + "The lightest of the kiln plates sit low on his snout.",
        "heavy_every": 2,
        "damage_bonus": 1
      ]),
      ([
        "name": "Firebank kilnback boar",
        "description":
            "Thick pieces of fired stone have fused along the boar's shoulder. He takes "
            + "time to gather his weight before driving a shoulder against the gate hard "
            + "enough to shake its brace.",
        "heavy_every": 4,
        "damage_bonus": 3
      ])
    }),
    "defeatTag": "kilnback-boar"
  ]);
}
