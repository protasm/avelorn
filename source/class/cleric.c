mapping definition() {
  return ([
    "id": "cleric",
    "name": "Cleric",
    "primary_stat": "wisdom",
    "starting_attributes": ([ "wisdom": 14, "constitution": 12, "charisma": 12 ]),
    "starter_weapon": "weapon/steel-mace",
    "resource": ([
      "name": "Faith", "base": 20, "per_level": 4,
      "per_stat": ([ "wisdom": 3 ])
    ]),
    "style_order": ({ "mercy", "zeal" }),
    "technique_order": ({ "smite", "mend", "sanctuary" }),
    "styles": ([
      "mercy": ([
        "name": "Mercy",
        "description": "Make healing and protection more effective.",
        "attack": -1, "defense": 1, "cost": 0, "support": 4
      ]),
      "zeal": ([
        "name": "Zeal",
        "description": "Focus your faith on delivering stronger attacks.",
        "attack": 2, "defense": 0, "cost": 1, "support": 0
      ])
    ]),
    "techniques": ([
      "smite": ([
        "name": "Smite",
        "description": "Strike a foe with a surge of faith.",
        "level": 1, "cost": 10, "effect": "damage", "power": 4
      ]),
      "mend": ([
        "name": "Mend",
        "description": "Restore health to yourself or a nearby ally.",
        "level": 3, "cost": 12, "effect": "heal", "power": 22
      ]),
      "sanctuary": ([
        "name": "Sanctuary",
        "description": "Call for protection for yourself or an ally.",
        "level": 6, "cost": 14, "effect": "guard", "power": 5
      ])
    ])
  ]);
}
