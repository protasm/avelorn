mapping definition() {
  return ([
    "id": "mage",
    "name": "Mage",
    "primary_stat": "intelligence",
    "starting_attributes": ([ "intelligence": 14, "wisdom": 12, "dexterity": 11 ]),
    "starter_weapon": "weapon/oak-focus-staff",
    "resource": ([
      "name": "Mana", "base": 20, "per_level": 4,
      "per_stat": ([ "intelligence": 3 ])
    ]),
    "style_order": ({ "ward", "channel" }),
    "technique_order": ({ "arcane-bolt", "arcane-ward", "unravel" }),
    "styles": ([
      "ward": ([
        "name": "Ward",
        "description": "Use your focus to hold off attacks.",
        "attack": 0, "defense": 1, "cost": 0, "support": 2
      ]),
      "channel": ([
        "name": "Channel",
        "description": "Focus on raw power at the cost of protection.",
        "attack": 3, "defense": -1, "cost": 2, "support": 0
      ])
    ]),
    "techniques": ([
      "arcane-bolt": ([
        "name": "Arcane Bolt",
        "description": "Strike a foe with a bolt of arcane fire.",
        "level": 1, "cost": 10, "effect": "damage", "power": 5
      ]),
      "arcane-ward": ([
        "name": "Arcane Ward",
        "description": "Wrap yourself or a nearby ally in a defensive field.",
        "level": 3, "cost": 12, "effect": "guard", "power": 4
      ]),
      "unravel": ([
        "name": "Unravel",
        "description": "Disrupt a foe's concentration and interrupt its attack.",
        "level": 6, "cost": 15, "effect": "stagger", "power": 3
      ])
    ])
  ]);
}
