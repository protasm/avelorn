mapping definition() {
  return ([
    "name": "Eastweir",
    "nation": "talvaren",
    "kind": "village",
    "anchor": ({ 2802, 3702 }),
    "entrance": "place/eastweir/arrival",
    "control": "dominion",
    "institution":
        "Canal houses maintain alternating flood watch and allocate the first safe pumping "
        + "shift.",
    "economy": "Waterwheels, alder pegs, clay, and kiln-fired sluice tile.",
    "rooms": ([
      "place/eastweir/arrival": ({ 30000070, "settlement" }),
      "place/eastweir/hearth": ({ 30000071, "interior" }),
      "place/eastweir/trade": ({ 30000072, "settlement" })
    ]),
    "routes": ([
      "eastweir-keepingstead": ([
        "name": "Weir Wagon",
        "mode": "coach",
        "from": "place/eastweir/arrival",
        "to": "place/keepingstead/arrival",
        "duration": 75,
        "fare": 0,
        "description": "A box of worn pegs travels back for the Keepingstead kiln to measure."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
