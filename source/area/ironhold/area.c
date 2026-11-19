mapping definition() {
  return ([
    "name": "Ironhold",
    "nation": "durnath",
    "kind": "city",
    "anchor": ({ 2320, 3800 }),
    "entrance": "place/ironhold/arrival",
    "control": "dominion",
    "institution":
        "The Dominion licenses the winches. The old bound companies still support injured "
        + "miners and their families.",
    "economy":
        "Ore, rope, measured stone, and foundry fittings. Senior bonded officers take their "
        + "portion before crews see a wage.",
    "rooms": ([
      "place/ironhold/arrival": ({ 30000030, "settlement" }),
      "place/ironhold/square": ({ 30000031, "settlement" }),
      "place/ironhold/market": ({ 30000032, "settlement" }),
      "place/ironhold/hall": ({ 30000033, "interior" }),
      "place/ironhold/hearth": ({ 30000034, "interior" }),
      "place/ironhold/yard": ({ 30000035, "settlement" })
    ]),
    "routes": ([
      "ironhold-freeharbor": ([
        "name": "Old Company Train",
        "mode": "caravan",
        "from": "place/ironhold/arrival",
        "to": "place/freeharbor/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "The train slows to tie each pack down before starting the descent to the salt "
            + "road."
      ]),
      "ironhold-witnessgate": ([
        "name": "Witness Freight Train",
        "mode": "caravan",
        "from": "place/ironhold/arrival",
        "to": "place/witnessgate/arrival",
        "duration": 120,
        "fare": 10,
        "description":
            "Loaded with cut stone, the wagon stays on the grade where passengers can stop "
            + "within sight of the witness flag."
      ]),
      "ironhold-keepingstead": ([
        "name": "Eastern Provision Train",
        "mode": "caravan",
        "from": "place/ironhold/arrival",
        "to": "place/keepingstead/arrival",
        "duration": 120,
        "fare": 10,
        "description": "The return train pulls sleds of replacement fittings toward the estuary."
      ]),
      "ironhold-ropebridge": ([
        "name": "Rope Company's Train",
        "mode": "caravan",
        "from": "place/ironhold/arrival",
        "to": "place/ropebridge/arrival",
        "duration": 75,
        "fare": 0,
        "description":
            "The train holds a practiced pause at the cleft while a rope-runner walks the tie "
            + "points."
      ]),
      "ironhold-lakeward": ([
        "name": "Great Lake Trail",
        "mode": "caravan",
        "from": "place/ironhold/arrival",
        "to": "place/lakeward/arrival",
        "duration": 80,
        "fare": 0,
        "description":
            "Ore gives way to stacks of raft stakes. The driver keeps the wheels near the "
            + "trees on the shore."
      ])
    ]),
    "trails": ([
      "ironhold-empty-winch": ([
        "from": "ironhold",
        "to": "empty-winch",
        "steps": ({ "east", "east", "east" }),
        "description":
            "Fresh boot prints pass a padlock eaten through with rust. The track north "
            + "crosses a broken axle embedded in the shale."
      ]),
      "ironhold-old-smelter": ([
        "from": "ironhold",
        "to": "old-smelter",
        "steps": ({ "north", "north" }),
        "description":
            "The path bears slips where a metal runner spun out with an ore pan. In the past "
            + "year someone drove guide pins deep into the stone."
      ])
    ]),
    "landmarks": ([ ])
  ]);
}
