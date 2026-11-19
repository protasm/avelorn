mapping definition() {
  return ([
    "name": "Press Yard",
    "nation": "caereth",
    "kind": "site",
    "anchor": ({ 2356, 3172 }),
    "entrance": "place/press-yard/approach",
    "control": "contested",
    "institution": "A relic of the levy camp still holds stocks tagged for compelled embarkations.",
    "economy": "Rigging and cargo hooks acquired under canceled pledges.",
    "rooms": ([
      "place/press-yard/approach": ({ 30000110, "wilderness" }),
      "place/press-yard/works": ({ 30000111, "dungeon" }),
      "place/press-yard/records": ({ 30000112, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x2356/y3172": ([
        "name": "Press Yard",
        "description":
            "A flight of shallow steps carries cuts intended to trip a loaded cart in wet "
            + "weather. The walls bear pitch at shoulder height where the old gate shuddered "
            + "into place."
      ])
    ]),
    "site": ([
      "area": "press-yard",
      "level": 10,
      "summary": "Clear the yard for families reclaiming goods extorted in the old levy."
    ])
  ]);
}
