mapping definition() {
  return ([
    "name": "Ropebridge",
    "nation": "durnath",
    "kind": "village",
    "anchor": ({ 2140, 3860 }),
    "entrance": "place/ropebridge/arrival",
    "control": "contested",
    "institution":
        "Bridge tenders answer for everyone they admit to the span, regardless of a "
        + "traveler's standing.",
    "economy": "Rope bridges, stone anchors, canvas, and guide services.",
    "rooms": ([
      "place/ropebridge/arrival": ({ 30000085, "settlement" }),
      "place/ropebridge/hearth": ({ 30000086, "interior" }),
      "place/ropebridge/trade": ({ 30000087, "settlement" })
    ]),
    "routes": ([
      "ropebridge-ironhold": ([
        "name": "Rope Company's Train",
        "mode": "caravan",
        "from": "place/ropebridge/arrival",
        "to": "place/ironhold/arrival",
        "duration": 75,
        "fare": 0,
        "description":
            "The company counts a sack of anchor pins and a passenger's small dog separately."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
