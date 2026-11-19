mapping definition() {
  return ([
    "name": "Driftwood",
    "nation": "caereth",
    "kind": "town",
    "anchor": ({ 2037, 3509 }),
    "entrance": "place/driftwood/arrival",
    "control": "contested",
    "institution": "Salvage crews renegotiate every voyage's share in a plank-roof assembly.",
    "economy": "Wreck salvage, reconditioned oak, and tackle.",
    "rooms": ([
      "place/driftwood/arrival": ({ 30000076, "settlement" }),
      "place/driftwood/hearth": ({ 30000077, "interior" }),
      "place/driftwood/trade": ({ 30000078, "settlement" })
    ]),
    "routes": ([
      "driftwood-freeharbor": ([
        "name": "Salvagers' Run",
        "mode": "ferry",
        "from": "place/driftwood/arrival",
        "to": "place/freeharbor/arrival",
        "duration": 80,
        "fare": 0,
        "description":
            "A loose spar lying under the bench keeps catching a passenger's boot on the way "
            + "back."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
