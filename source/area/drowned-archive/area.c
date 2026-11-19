mapping definition() {
  return ([
    "name": "Drowned Archive",
    "nation": "ordel",
    "kind": "site",
    "anchor": ({ 2520, 4375 }),
    "entrance": "place/drowned-archive/approach",
    "control": "contested",
    "institution": "A ledger chamber beneath the flooded court held two sets of public records.",
    "economy": "Wooden testimony rolls, salvaged doors, and slow-moving channels.",
    "rooms": ([
      "place/drowned-archive/approach": ({ 30000104, "wilderness" }),
      "place/drowned-archive/works": ({ 30000105, "dungeon" }),
      "place/drowned-archive/records": ({ 30000106, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x2520/y4375": ([
        "name": "Drowned Archive",
        "description":
            "Flood readings are cut into a wind-smoothed beam. The lowest doors show where "
            + "the most recent tide stopped."
      ])
    ]),
    "site": ([
      "area": "drowned-archive",
      "level": 10,
      "summary":
          "Recover the flooded witness chamber when the tide pushes its denizen onto dry "
          + "ground."
    ])
  ]);
}
