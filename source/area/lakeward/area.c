mapping definition() {
  return ([
    "name": "Lakeward",
    "nation": "durnath",
    "kind": "town",
    "anchor": ({ 2289, 3897 }),
    "entrance": "place/lakeward/arrival",
    "control": "resistance",
    "institution":
        "Mining families take turns provisioning strangers who reach town by way of the old "
        + "shore.",
    "economy": "Freshwater fisheries, timber rafts, and repairs for boiler-driven quarry gear.",
    "rooms": ([
      "place/lakeward/arrival": ({ 30000088, "settlement" }),
      "place/lakeward/hearth": ({ 30000089, "interior" }),
      "place/lakeward/trade": ({ 30000090, "settlement" })
    ]),
    "routes": ([
      "lakeward-ironhold": ([
        "name": "Great Lake Trail",
        "mode": "caravan",
        "from": "place/lakeward/arrival",
        "to": "place/ironhold/arrival",
        "duration": 80,
        "fare": 0,
        "description":
            "The lake train joins the main track behind an easy bend so that no lookout "
            + "mistakes it for more ore."
      ]),
      "lakeward-coalstair": ([
        "name": "Coalstair Pack Train",
        "mode": "caravan",
        "from": "place/lakeward/arrival",
        "to": "place/coalstair/arrival",
        "duration": 105,
        "fare": 0,
        "description":
            "The guides stop to check the first cairn and keep passengers close together for "
            + "the final rise."
      ])
    ]),
    "trails": ([
      "lakeward-cinder-bank": ([
        "from": "lakeward",
        "to": "cinder-bank",
        "steps": ({ "east", "east", "east" }),
        "description": "Follow the low branch trail above the eastern shore."
      ])
    ]),
    "landmarks": ([ ])
  ]);
}
