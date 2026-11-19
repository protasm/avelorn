mapping definition() {
  return ([
    "name": "Saltbreak",
    "nation": "talvaren",
    "kind": "village",
    "anchor": ({ 3326, 3454 }),
    "entrance": "place/saltbreak/arrival",
    "control": "contested",
    "institution":
        "A keeping-house keeps spare pilings and a month's grain behind each rebuilt seawall.",
    "economy": "Tidewall construction, brined goods, and sea rescue gear.",
    "rooms": ([
      "place/saltbreak/arrival": ({ 30000073, "settlement" }),
      "place/saltbreak/hearth": ({ 30000074, "interior" }),
      "place/saltbreak/trade": ({ 30000075, "settlement" })
    ]),
    "routes": ([
      "saltbreak-keepingstead": ([
        "name": "Seawall Lighter",
        "mode": "ferry",
        "from": "place/saltbreak/arrival",
        "to": "place/keepingstead/arrival",
        "duration": 70,
        "fare": 0,
        "description":
            "The crew of the lighter retrieves one of Saltbreak's spare rescue lines and sets "
            + "off to deliver it."
      ])
    ]),
    "trails": ([ ]),
    "landmarks": ([ ])
  ]);
}
