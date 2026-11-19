mapping definition() {
  return ([
    "name": "Keepingstead",
    "nation": "talvaren",
    "kind": "city",
    "anchor": ({ 3033, 3856 }),
    "entrance": "place/keepingstead/arrival",
    "control": "dominion",
    "institution":
        "The Keeping-House maintains a public reserve and repairs common works. The "
        + "requisition office counts the same grain for the garrison.",
    "economy":
        "Preserved grain, orchard stock, clay tile, and canal maintenance. Work crews earn "
        + "priority for household reserves.",
    "rooms": ([
      "place/keepingstead/arrival": ({ 30000018, "settlement" }),
      "place/keepingstead/square": ({ 30000019, "settlement" }),
      "place/keepingstead/market": ({ 30000020, "settlement" }),
      "place/keepingstead/hall": ({ 30000021, "interior" }),
      "place/keepingstead/hearth": ({ 30000022, "interior" }),
      "place/keepingstead/yard": ({ 30000023, "settlement" })
    ]),
    "routes": ([
      "keepingstead-witnessgate": ([
        "name": "Common Reserve Packet",
        "mode": "ferry",
        "from": "place/keepingstead/arrival",
        "to": "place/witnessgate/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "A sealed set of reserve accounts is handed up behind you. At the northern "
            + "harbor, two clerks independently receive it."
      ]),
      "keepingstead-freeharbor": ([
        "name": "South Coast Loop",
        "mode": "ferry",
        "from": "place/keepingstead/arrival",
        "to": "place/freeharbor/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "Stone ballast keeps the boat steady along a stretch of cliff. The crew speaks "
            + "about what they each agreed to carry south."
      ]),
      "keepingstead-ironhold": ([
        "name": "Eastern Provision Train",
        "mode": "caravan",
        "from": "place/keepingstead/arrival",
        "to": "place/ironhold/arrival",
        "duration": 120,
        "fare": 10,
        "description":
            "Tile carts give way to an iron-cleared road that reaches the oldest foundries. "
            + "Reserve sacks rattle against wooden stakes."
      ]),
      "keepingstead-orchard-cross": ([
        "name": "Rootstock Coach",
        "mode": "coach",
        "from": "place/keepingstead/arrival",
        "to": "place/orchard-cross/arrival",
        "duration": 65,
        "fare": 0,
        "description": "Young trees travel upright where the rail can shelter them from the wind."
      ]),
      "keepingstead-eastweir": ([
        "name": "Weir Wagon",
        "mode": "coach",
        "from": "place/keepingstead/arrival",
        "to": "place/eastweir/arrival",
        "duration": 75,
        "fare": 0,
        "description":
            "A wagon follows the fresh tile track until the first picket of river alder marks "
            + "the pumphouse lands."
      ]),
      "keepingstead-saltbreak": ([
        "name": "Seawall Lighter",
        "mode": "ferry",
        "from": "place/keepingstead/arrival",
        "to": "place/saltbreak/arrival",
        "duration": 70,
        "fare": 0,
        "description": "Cords from the high seawall lower a basket for the crew's bread."
      ])
    ]),
    "trails": ([
      "keepingstead-silt-house": ([
        "from": "keepingstead",
        "to": "silt-house",
        "steps": ({ "west", "west", "west" }),
        "description":
            "An embankment path rises north to a timber sash gate. The ditch beside it leaves "
            + "a heavy clay mark above the usual water line."
      ]),
      "keepingstead-red-run": ([
        "from": "keepingstead",
        "to": "red-run",
        "steps": ({ "south", "south" }),
        "description":
            "A wet stripe glows red where the bank was patched with the kiln's last batch. "
            + "The rest of the path carries the bootprints of workers called to shift early."
      ])
    ]),
    "landmarks": ([ ])
  ]);
}
