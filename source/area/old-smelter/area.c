mapping definition() {
  return ([
    "name": "Old Smelter",
    "nation": "durnath",
    "kind": "site",
    "anchor": ({ 2320, 3802 }),
    "entrance": "place/old-smelter/approach",
    "control": "contested",
    "institution": "The early furnaces gave tools to people who could not buy them on the coast.",
    "economy": "Charcoal, ordinary iron, and high-temperature fittings.",
    "rooms": ([
      "place/old-smelter/approach": ({ 30000113, "wilderness" }),
      "place/old-smelter/works": ({ 30000114, "dungeon" }),
      "place/old-smelter/records": ({ 30000115, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x2320/y3802": ([
        "name": "Old Smelter",
        "description":
            "The path bears slips where a metal runner spun out with an ore pan. In the past "
            + "year someone drove guide pins deep into the stone."
      ])
    ]),
    "site": ([
      "area": "old-smelter",
      "level": 10,
      "summary": "Recover the lower foundry for workers who stopped receiving provisions."
    ])
  ]);
}
