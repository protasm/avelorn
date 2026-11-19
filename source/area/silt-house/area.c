mapping definition() {
  return ([
    "name": "Old Silt House",
    "nation": "talvaren",
    "kind": "site",
    "anchor": ({ 3030, 3856 }),
    "entrance": "place/silt-house/approach",
    "control": "dominion",
    "institution": "Sluice wardens share responsibility for keeping low stores dry.",
    "economy": "Overflow control protects the lowest storehouses and the quay.",
    "rooms": ([
      "place/silt-house/approach": ({ 30000042, "wilderness" }),
      "place/silt-house/works": ({ 30000043, "dungeon" }),
      "place/silt-house/records": ({ 30000044, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x3030/y3856": ([
        "name": "Old Silt House",
        "description":
            "An embankment path rises north to a timber sash gate. The ditch beside it leaves "
            + "a heavy clay mark above the usual water line."
      ])
    ]),
    "site": ([
      "area": "silt-house",
      "level": 5,
      "summary": "Free the common drain before the garrison can use a flood to seize the reserve."
    ])
  ]);
}
