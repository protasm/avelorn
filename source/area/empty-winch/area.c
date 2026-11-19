mapping definition() {
  return ([
    "name": "Empty Winch",
    "nation": "durnath",
    "kind": "site",
    "anchor": ({ 2323, 3800 }),
    "entrance": "place/empty-winch/approach",
    "control": "contested",
    "institution": "A company of rope workers used to test and publicly cut worn rope here.",
    "economy": "Disused above ground, the lower works still pass supplies to the deep shafts.",
    "rooms": ([
      "place/empty-winch/approach": ({ 30000048, "wilderness" }),
      "place/empty-winch/works": ({ 30000049, "dungeon" }),
      "place/empty-winch/records": ({ 30000050, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x2323/y3800": ([
        "name": "Empty Winch",
        "description":
            "Fresh boot prints pass a padlock eaten through with rust. The track north "
            + "crosses a broken axle embedded in the shale."
      ])
    ]),
    "site": ([
      "area": "empty-winch",
      "level": 8,
      "summary":
          "Break the watch at the Empty Winch and expose the unregistered work below the "
          + "mountain."
    ])
  ]);
}
