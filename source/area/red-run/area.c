mapping definition() {
  return ([
    "name": "Red Run",
    "nation": "talvaren",
    "kind": "site",
    "anchor": ({ 3033, 3854 }),
    "entrance": "place/red-run/approach",
    "control": "contested",
    "institution": "Local crews maintain tidewall repairs in marked sections.",
    "economy": "Washed stone, tile, and relaid drains for the coastal granaries.",
    "rooms": ([
      "place/red-run/approach": ({ 30000107, "wilderness" }),
      "place/red-run/works": ({ 30000108, "dungeon" }),
      "place/red-run/records": ({ 30000109, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x3033/y3854": ([
        "name": "Red Run",
        "description":
            "A wet stripe glows red where the bank was patched with the kiln's last batch. "
            + "The rest of the path carries the bootprints of workers called to shift early."
      ])
    ]),
    "site": ([
      "area": "red-run",
      "level": 10,
      "summary": "Protect the running water from the wildlife of a too-deep spillway."
    ])
  ]);
}
