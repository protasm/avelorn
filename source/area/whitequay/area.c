mapping definition() {
  return ([
    "name": "Whitequay",
    "nation": "ordel",
    "kind": "town",
    "anchor": ({ 2395, 4685 }),
    "entrance": "place/whitequay/arrival",
    "control": "dominion",
    "institution":
        "The customs hall reads arrivals into a witnessed register before they are charged.",
    "economy": "Seaborne lumber, lime mortar, blue ink, and cloth seals.",
    "rooms": ([
      "place/whitequay/arrival": ({ 30000064, "settlement" }),
      "place/whitequay/hearth": ({ 30000065, "interior" }),
      "place/whitequay/trade": ({ 30000066, "settlement" })
    ]),
    "routes": ([
      "whitequay-witnessgate": ([
        "name": "Whitequay Cutter",
        "mode": "ferry",
        "from": "place/whitequay/arrival",
        "to": "place/witnessgate/arrival",
        "duration": 60,
        "fare": 0,
        "description":
            "The cutter passes three households fishing and invites each to stand witness to "
            + "the cargo count."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
