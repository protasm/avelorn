mapping definition() {
  return ([
    "name": "Havenrode",
    "nation": "arvenna",
    "kind": "city",
    "anchor": ({ 812, 3756 }),
    "entrance": "place/havenrode/arrival",
    "control": "dominion",
    "institution":
        "Sponsor households apportion shelter for guests and hold hearings in the Open Roof "
        + "Hall.",
    "economy":
        "Fishing, caulking, wool, and tariffs on visiting cargo. The quay laborers keep a "
        + "separate wage ledger.",
    "rooms": ([
      "place/havenrode/arrival": ({ 30000006, "settlement" }),
      "place/havenrode/square": ({ 30000007, "settlement" }),
      "place/havenrode/market": ({ 30000008, "settlement" }),
      "place/havenrode/hall": ({ 30000009, "interior" }),
      "place/havenrode/hearth": ({ 30000010, "interior" }),
      "place/havenrode/yard": ({ 30000011, "settlement" })
    ]),
    "routes": ([
      "havenrode-westwatch": ([
        "name": "Blue Lamp Ferry",
        "mode": "ferry",
        "from": "place/havenrode/arrival",
        "to": "place/westwatch/landing",
        "duration": 120,
        "fare": 0,
        "description":
            "The pilot puts the houses of Havenrode behind the stern and searches for the "
            + "blue light at Westwatch."
      ]),
      "havenrode-witnessgate": ([
        "name": "North Coast Packet",
        "mode": "ferry",
        "from": "place/havenrode/arrival",
        "to": "place/witnessgate/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "Tarred bundles turn under the boom as the packet leaves the Arvenni houses for "
            + "the tall posts of Witnessgate."
      ]),
      "havenrode-freeharbor": ([
        "name": "West–South Coastal Packet",
        "mode": "ferry",
        "from": "place/havenrode/arrival",
        "to": "place/freeharbor/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "The pilot traces a long coastal reach where most of the lights belong to fishing "
            + "families rather than the government."
      ]),
      "havenrode-greenhame": ([
        "name": "Dune Coach",
        "mode": "coach",
        "from": "place/havenrode/arrival",
        "to": "place/greenhame/arrival",
        "duration": 60,
        "fare": 0,
        "description":
            "At a sandy turning post the driver picks up a bundle of wool cut with dune grit."
      ]),
      "havenrode-saltmere": ([
        "name": "Saltmere Tender",
        "mode": "ferry",
        "from": "place/havenrode/arrival",
        "to": "place/saltmere/arrival",
        "duration": 65,
        "fare": 0,
        "description":
            "The tender waits for a wave to lift it over the last flat bar before tying up at "
            + "the Saltmere Ledge."
      ])
    ]),
    "trails": ([
      "havenrode-salt-ledger": ([
        "from": "havenrode",
        "to": "salt-ledger",
        "steps": ({ "west", "west", "west" }),
        "description":
            "Fish hooks hang from a broken boundary line. The walk north leads to a "
            + "counting-house built on the site of a communal meal shed."
      ]),
      "havenrode-bond-yard": ([
        "from": "havenrode",
        "to": "bond-yard",
        "steps": ({ "south", "south" }),
        "description":
            "Iron spear rests stand outside the loan house. A shuttered window carries the "
            + "old symbol for an empty bed offered to a stranger."
      ])
    ]),
    "landmarks": ([ ])
  ]);
}
