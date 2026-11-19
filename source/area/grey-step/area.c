mapping definition() {
  return ([
    "name": "Grey Step",
    "nation": "caereth",
    "kind": "village",
    "anchor": ({ 2171, 3393 }),
    "entrance": "place/grey-step/arrival",
    "control": "contested",
    "institution":
        "Crews leave a sill unmarked when they decline a levy. Blank space is a public record.",
    "economy": "Beach forage, boat repairs, and overtime for the inland mail relay.",
    "rooms": ([
      "place/grey-step/arrival": ({ 30000079, "settlement" }),
      "place/grey-step/hearth": ({ 30000080, "interior" }),
      "place/grey-step/trade": ({ 30000081, "settlement" })
    ]),
    "routes": ([
      "grey-step-freeharbor": ([
        "name": "Grey Relay",
        "mode": "coach",
        "from": "place/grey-step/arrival",
        "to": "place/freeharbor/arrival",
        "duration": 65,
        "fare": 0,
        "description":
            "A partner waiting on the first grey stone takes the outgoing post in the middle "
            + "of the ride."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
