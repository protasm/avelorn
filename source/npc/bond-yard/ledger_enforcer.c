inherit "npc/hostile";

mapping definition() {
  return ([
    "kind": "hostile",
    "name": "Ledger Enforcer",
    "gender": "male",
    "role": "enemy",
    "description": "The enforcer bears the outline of a guest-house tile on his black steel mask.",
    "aliases": ({ "enforcer" }),
    "level": 10,
    "health": 135,
    "minimumDamage": 6,
    "maximumDamage": 10,
    "experience": 150,
    "copper": 42,
    "variants": ({
      ([
        "name": "Ledger Enforcer of the Lock Watch",
        "description":
            "The enforcer holds a short iron shield on its edge, ready to bring it down "
            + "on short notice. A row of borrowed house keys rattles beneath the grip.",
        "heavy_every": 2,
        "damage_bonus": 0
      ]),
      ([
        "name": "Ledger Enforcer of the Debt Sweep",
        "description":
            "A sledge meant for breaking door bars rests across the old guest-house mark "
            + "on his mask. He walks an intruder back toward the wall before pulling it "
            + "into a swing.",
        "heavy_every": 4,
        "damage_bonus": 3
      ])
    }),
    "defeatTag": "ledger-enforcer"
  ]);
}
