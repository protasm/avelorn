mapping definition() {
  return ([
    "name": "Coalstair",
    "nation": "durnath",
    "kind": "village",
    "anchor": ({ 2395, 3726 }),
    "entrance": "place/coalstair/arrival",
    "control": "resistance",
    "institution":
        "Mountain guides maintain a shelter at the point where mineral weather can hide the "
        + "path.",
    "economy": "Ice gear, rope, charcoal, and pack animal handling.",
    "rooms": ([
      "place/coalstair/arrival": ({ 30000091, "settlement" }),
      "place/coalstair/hearth": ({ 30000092, "interior" }),
      "place/coalstair/trade": ({ 30000093, "settlement" })
    ]),
    "routes": ([
      "coalstair-lakeward": ([
        "name": "Coalstair Pack Train",
        "mode": "caravan",
        "from": "place/coalstair/arrival",
        "to": "place/lakeward/arrival",
        "duration": 105,
        "fare": 0,
        "description":
            "Pack animals bring charcoal sacks back from the high shelter and stamp patiently "
            + "on the lake road."
      ])
    ]),
    "trails": ([
      "coalstair-deep-vault": ([
        "from": "coalstair",
        "to": "deep-vault",
        "steps": ({
          "east", "east", "east", "east", "east", "east", "east", "east", "east", "east", "east",
          "east", "east", "east", "east", "east", "east", "south", "south", "south", "south",
          "south", "south"
        }),
        "description":
            "Follow the iron cairns seventeen squares east, then six squares south to the "
            + "vault mouth."
      ])
    ]),
    "landmarks": ([
      "place/atlas/x2395/y3726": ([
        "name": "Coalstair Cairn",
        "description": "A first cairn points up the eastbound track toward the lip of the mountain."
      ]),
      "place/atlas/x2400/y3726": ([
        "name": "Old Load Rest",
        "description":
            "Empty gear supports form a bend where guides used to shoulder packs. The chain "
            + "of cairns continues east."
      ]),
      "place/atlas/x2406/y3726": ([
        "name": "Rope Traverse",
        "description":
            "An iron loop faces the gap to the south. Cairns go east, uphill toward a shelf "
            + "of old snow."
      ]),
      "place/atlas/x2412/y3726": ([
        "name": "South Turn",
        "description":
            "The cairns turn south along the snow line. Someone recently marked the highest "
            + "one with a streak of coal."
      ])
    ])
  ]);
}
