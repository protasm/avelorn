mapping definition() {
  return ([
    "name": "Reedford",
    "nation": "ordel",
    "kind": "town",
    "anchor": ({ 2040, 4177 }),
    "entrance": "place/reedford/arrival",
    "control": "dominion",
    "institution": "An open witness stand settles disputes about reed beds and use of the causeway.",
    "economy": "Reed mats, raised walkways, smoked eels, and narrow freight carts.",
    "rooms": ([
      "place/reedford/arrival": ({ 30000061, "settlement" }),
      "place/reedford/hearth": ({ 30000062, "interior" }),
      "place/reedford/trade": ({ 30000063, "settlement" })
    ]),
    "routes": ([
      "reedford-witnessgate": ([
        "name": "Reed Causeway Coach",
        "mode": "coach",
        "from": "place/reedford/arrival",
        "to": "place/witnessgate/arrival",
        "duration": 80,
        "fare": 0,
        "description":
            "A carpenter calls the arrival of dry ground so the driver can increase the pace."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
