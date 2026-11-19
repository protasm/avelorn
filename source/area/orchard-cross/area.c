mapping definition() {
  return ([
    "name": "Orchard Cross",
    "nation": "talvaren",
    "kind": "town",
    "anchor": ({ 2842, 3920 }),
    "entrance": "place/orchard-cross/arrival",
    "control": "contested",
    "institution": "Each family holds a share of the common root cellar and owes an annual graft.",
    "economy": "Apples, seed stock, spades, and slow-dried firewood.",
    "rooms": ([
      "place/orchard-cross/arrival": ({ 30000067, "settlement" }),
      "place/orchard-cross/hearth": ({ 30000068, "interior" }),
      "place/orchard-cross/trade": ({ 30000069, "settlement" })
    ]),
    "routes": ([
      "orchard-cross-keepingstead": ([
        "name": "Rootstock Coach",
        "mode": "coach",
        "from": "place/orchard-cross/arrival",
        "to": "place/keepingstead/arrival",
        "duration": 65,
        "fare": 0,
        "description":
            "A grower names the best apples in every crate the coach takes to Keepingstead."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
