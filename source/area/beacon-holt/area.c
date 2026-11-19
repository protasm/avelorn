mapping definition() {
  return ([
    "name": "Beacon Holt",
    "nation": "ordel",
    "kind": "village",
    "anchor": ({ 2282, 4711 }),
    "entrance": "place/beacon-holt/arrival",
    "control": "contested",
    "institution":
        "Northern beacon keepers make separate public attestations of the signals they "
        + "receive.",
    "economy": "Pine resin, lamp wicks, and repaired lantern glass.",
    "rooms": ([
      "place/beacon-holt/arrival": ({ 30000058, "settlement" }),
      "place/beacon-holt/hearth": ({ 30000059, "interior" }),
      "place/beacon-holt/trade": ({ 30000060, "settlement" })
    ]),
    "routes": ([
      "beacon-holt-witnessgate": ([
        "name": "Holt Coach",
        "mode": "coach",
        "from": "place/beacon-holt/arrival",
        "to": "place/witnessgate/arrival",
        "duration": 75,
        "fare": 0,
        "description": "Two beacons go dark behind you before the coach reaches the witness stones."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
