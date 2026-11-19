mapping definition() {
  return ([
    "name": "Tide Chapel",
    "nation": "caereth",
    "kind": "site",
    "anchor": ({ 2353, 3170 }),
    "entrance": "place/tide-chapel/approach",
    "control": "contested",
    "institution": "Fishing crews agreed to embarkations here before they owned a charter.",
    "economy": "Contract crews still trade salvage shares.",
    "rooms": ([
      "place/tide-chapel/approach": ({ 30000045, "wilderness" }),
      "place/tide-chapel/works": ({ 30000046, "dungeon" }),
      "place/tide-chapel/records": ({ 30000047, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x2353/y3170": ([
        "name": "Tide Chapel",
        "description":
            "A red pennant warns travelers to stay clear of a stretch of tide-dark stone. The "
            + "old meeting shelter opens north into the hillside."
      ])
    ]),
    "site": ([
      "area": "tide-chapel",
      "level": 6,
      "summary": "Stop the transfer of prisoners misidentified as volunteer work crews."
    ])
  ]);
}
