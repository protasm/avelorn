mapping definition() {
  return ([
    "id": "ranger",
    "name": "Ranger",
    "primary_stat": "dexterity",
    "starting_attributes": ([ "dexterity": 14, "constitution": 12, "wisdom": 12 ]),
    "starter_weapon": "weapon/ashwood-shortbow",
    "resource": ([
      "name": "Stamina", "base": 25, "per_level": 3,
      "per_stat": ([ "dexterity": 1, "constitution": 1 ])
    ]),
    "style_order": ({ "scout", "hunter" }),
    "technique_order": ({ "aimed-shot", "hamstring", "field-dressing" }),
    "styles": ([
      "scout": ([
        "name": "Scout",
        "description": "Keep light on your feet and make field treatment easier.",
        "attack": -1, "defense": 1, "cost": 0, "support": 2
      ]),
      "hunter": ([
        "name": "Hunter",
        "description": "Press an opening at increased exertion.",
        "attack": 2, "defense": 0, "cost": 1, "support": 0
      ])
    ]),
    "techniques": ([
      "aimed-shot": ([
        "name": "Aimed Shot",
        "description": "Pick out an exposed target.",
        "level": 1, "cost": 8, "effect": "damage", "power": 4
      ]),
      "hamstring": ([
        "name": "Hamstring",
        "description": "Interrupt a foe with a low strike.",
        "level": 3, "cost": 11, "effect": "stagger", "power": 1
      ]),
      "field-dressing": ([
        "name": "Field Dressing",
        "description": "Tend your own wounds or a nearby ally's.",
        "level": 6, "cost": 12, "effect": "heal", "power": 16
      ])
    ])
  ]);
}
