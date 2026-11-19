mapping definition() {
  return ([
    "name": "Moorcross",
    "nation": "caereth",
    "kind": "village",
    "anchor": ({ 2321, 3483 }),
    "entrance": "place/moorcross/arrival",
    "control": "dominion",
    "institution":
        "Stock keepers reserve the right to withdraw animals from the government pack trains.",
    "economy": "Sheep, rough grey felt, fodder, and pasture leases.",
    "rooms": ([
      "place/moorcross/arrival": ({ 30000082, "settlement" }),
      "place/moorcross/hearth": ({ 30000083, "interior" }),
      "place/moorcross/trade": ({ 30000084, "settlement" })
    ]),
    "routes": ([
      "moorcross-freeharbor": ([
        "name": "Moor Harness Coach",
        "mode": "coach",
        "from": "place/moorcross/arrival",
        "to": "place/freeharbor/arrival",
        "duration": 70,
        "fare": 0,
        "description":
            "Drovers call the last turn from the slope as the track leaves the wet country."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
