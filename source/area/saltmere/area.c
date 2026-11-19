mapping definition() {
  return ([
    "name": "Saltmere",
    "nation": "arvenna",
    "kind": "village",
    "anchor": ({ 1097, 3844 }),
    "entrance": "place/saltmere/arrival",
    "control": "dominion",
    "institution": "Families shelter stranded crews for one night without requiring a sponsor.",
    "economy": "Salt pans, preserved herring, and long shallow-bottomed tenders.",
    "rooms": ([
      "place/saltmere/arrival": ({ 30000055, "settlement" }),
      "place/saltmere/hearth": ({ 30000056, "interior" }),
      "place/saltmere/trade": ({ 30000057, "settlement" })
    ]),
    "routes": ([
      "saltmere-havenrode": ([
        "name": "Saltmere Tender",
        "mode": "ferry",
        "from": "place/saltmere/arrival",
        "to": "place/havenrode/arrival",
        "duration": 65,
        "fare": 0,
        "description":
            "Empty salt wraps keep bumping the cabin until the boat turns into deeper water."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
