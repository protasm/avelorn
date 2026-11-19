mapping definition() {
  return ([
    "name": "Witnessgate",
    "nation": "ordel",
    "kind": "city",
    "anchor": ({ 2520, 4377 }),
    "entrance": "place/witnessgate/arrival",
    "control": "dominion",
    "institution":
        "Public witnesses speak for written obligations. Off the books, other witnesses "
        + "preserve testimony about families removed from the harbor.",
    "economy":
        "Records, blue ink, timbers, and bonded storage. The seal trade furnishes papers as "
        + "well as stamps.",
    "rooms": ([
      "place/witnessgate/arrival": ({ 30000012, "settlement" }),
      "place/witnessgate/square": ({ 30000013, "settlement" }),
      "place/witnessgate/market": ({ 30000014, "settlement" }),
      "place/witnessgate/hall": ({ 30000015, "interior" }),
      "place/witnessgate/hearth": ({ 30000016, "interior" }),
      "place/witnessgate/yard": ({ 30000017, "settlement" })
    ]),
    "routes": ([
      "witnessgate-havenrode": ([
        "name": "North Coast Packet",
        "mode": "ferry",
        "from": "place/witnessgate/arrival",
        "to": "place/havenrode/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "A clerk reads the passengers' names once before the open sea, then again as the "
            + "first green roofs of Havenrode appear."
      ]),
      "witnessgate-keepingstead": ([
        "name": "Common Reserve Packet",
        "mode": "ferry",
        "from": "place/witnessgate/arrival",
        "to": "place/keepingstead/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "The vessel carries empty grain sacks to a coast where every other roof seems to "
            + "cover a storehouse."
      ]),
      "witnessgate-ironhold": ([
        "name": "Witness Freight Train",
        "mode": "caravan",
        "from": "place/witnessgate/arrival",
        "to": "place/ironhold/arrival",
        "duration": 120,
        "fare": 10,
        "description":
            "A timber wagon follows the inland companies carrying records of what the "
            + "mountains delivered last winter."
      ]),
      "witnessgate-beacon-holt": ([
        "name": "Holt Coach",
        "mode": "coach",
        "from": "place/witnessgate/arrival",
        "to": "place/beacon-holt/arrival",
        "duration": 75,
        "fare": 0,
        "description":
            "The driver asks a passenger to carry the lamp oil so it stays upright on the "
            + "last, deeply pitted climb."
      ]),
      "witnessgate-reedford": ([
        "name": "Reed Causeway Coach",
        "mode": "coach",
        "from": "place/witnessgate/arrival",
        "to": "place/reedford/arrival",
        "duration": 80,
        "fare": 0,
        "description":
            "The coach inspects the new planking before crossing the wettest stretch. Bundles "
            + "of reeds slide under your feet."
      ]),
      "witnessgate-whitequay": ([
        "name": "Whitequay Cutter",
        "mode": "ferry",
        "from": "place/witnessgate/arrival",
        "to": "place/whitequay/arrival",
        "duration": 60,
        "fare": 0,
        "description": "A runner sets a row of chalk boards face down for the crossing."
      ])
    ]),
    "trails": ([
      "witnessgate-record-vault": ([
        "from": "witnessgate",
        "to": "record-vault",
        "steps": ({ "west", "west", "west" }),
        "description":
            "Flood scratches at eye level show where the river once reached the archive. A "
            + "badly fitted door faces north, and wax has spilled over its sill."
      ]),
      "witnessgate-drowned-archive": ([
        "from": "witnessgate",
        "to": "drowned-archive",
        "steps": ({ "south", "south" }),
        "description":
            "Flood readings are cut into a wind-smoothed beam. The lowest doors show where "
            + "the most recent tide stopped."
      ])
    ]),
    "landmarks": ([ ])
  ]);
}
