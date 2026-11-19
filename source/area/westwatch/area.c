mapping definition() {
  return ([
    "name": "Westwatch",
    "nation": "arvenna",
    "kind": "village",
    "anchor": ({ 113, 3336 }),
    "entrance": "place/westwatch/landing",
    "control": "contested",
    "institution": "A guest-house staffed by fishing families who sponsor travelers.",
    "economy": "Pilotage, driftwood repairs, dried fish, and travel supplies.",
    "rooms": ([
      "place/westwatch/guest-house": ({ 30000001, "interior" }),
      "place/westwatch/landing": ({ 30000002, "settlement" }),
      "place/westwatch/coopers-yard": ({ 30000003, "settlement" })
    ]),
    "routes": ([
      "westwatch-havenrode": ([
        "name": "Blue Lamp Ferry",
        "mode": "ferry",
        "from": "place/westwatch/landing",
        "to": "place/havenrode/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "The pilot follows the fish-bone markers around the western dunes and steers up "
            + "the Arvenni coast."
      ])
    ]),
    "trails": ([
      "westwatch-signal": ([
        "from": "westwatch",
        "to": "headland-signal",
        "steps": ({ "east" }),
        "description": "Follow the dune ridge east to the pilot's signal."
      ])
    ]),
    "landmarks": ([
      "place/atlas/x0113/y3336": ([
        "name": "Westwatch Dunes",
        "description":
            "A track from the landing turns east beside a row of fish-bone markers, toward an "
            + "old signal lamp."
      ])
    ])
  ]);
}
