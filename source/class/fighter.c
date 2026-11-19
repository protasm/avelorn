mapping definition() {
  return ([
    "id": "fighter",
    "name": "Fighter",
    "primary_stat": "strength",
    "starting_attributes": ([ "strength": 14, "constitution": 13, "dexterity": 11 ]),
    "starter_weapon": "weapon/arming-sword",
    "resource": ([
      "name": "Stamina", "base": 20, "per_level": 3,
      "per_stat": ([ "constitution": 2 ])
    ]),
    "style_order": ({ "guard", "assault" }),
    "technique_order": ({ "mighty-blow", "shield-wall", "pommel-strike" }),
    "styles": ([
      "guard": ([
        "name": "Guard",
        "description": "Favor protection over stronger strikes.",
        "attack": -1, "defense": 2, "cost": 0, "support": 1
      ]),
      "assault": ([
        "name": "Assault",
        "description": "Commit to heavier attacks and leave openings.",
        "attack": 2, "defense": -1, "cost": 1, "support": 0
      ])
    ]),
    "techniques": ([
      "mighty-blow": ([
        "name": "Mighty Blow",
        "description": "Strike with the full weight of your weapon.",
        "level": 1, "cost": 8, "effect": "damage", "power": 4
      ]),
      "shield-wall": ([
        "name": "Shield Wall",
        "description": "Protect yourself or a nearby ally.",
        "level": 3, "cost": 10, "effect": "guard", "power": 4
      ]),
      "pommel-strike": ([
        "name": "Pommel Strike",
        "description": "Interrupt a foe's next attack.",
        "level": 6, "cost": 13, "effect": "stagger", "power": 2
      ])
    ])
  ]);
}
