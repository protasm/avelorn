mapping definition() {
  return ([
    "name": "Greenhame",
    "nation": "arvenna",
    "kind": "town",
    "anchor": ({ 350, 3542 }),
    "entrance": "place/greenhame/arrival",
    "control": "contested",
    "institution":
        "Foster-house keepers file apprenticeships alongside births, debts, and inheritances.",
    "economy": "Coarse cloth, dune honey, and repairs to wagon axles.",
    "rooms": ([
      "place/greenhame/arrival": ({ 30000051, "settlement" }),
      "place/greenhame/hearth": ({ 30000052, "interior" }),
      "place/greenhame/trade": ({ 30000053, "settlement" }),
      "place/greenhame/list-room": ({ 30000054, "interior" })
    ]),
    "routes": ([
      "greenhame-havenrode": ([
        "name": "Dune Coach",
        "mode": "coach",
        "from": "place/greenhame/arrival",
        "to": "place/havenrode/arrival",
        "duration": 60,
        "fare": 0,
        "description":
            "The road leaves the foster plots and curls back toward the sound of saws at the "
            + "quay."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
