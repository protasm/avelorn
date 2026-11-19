mapping definition() {
  return ([
    "name": "Freeharbor",
    "nation": "caereth",
    "kind": "city",
    "anchor": ({ 2356, 3170 }),
    "entrance": "place/freeharbor/arrival",
    "control": "contested",
    "institution":
        "A renewal assembly meets whenever five households call for another vote. Those who "
        + "can afford the fines refuse to sign the Dominion pledge.",
    "economy":
        "Salt exports, pilot contracts, rope, and small ship repairs. Fisher crews bargain "
        + "individually rather than answer to a single guild.",
    "rooms": ([
      "place/freeharbor/arrival": ({ 30000024, "settlement" }),
      "place/freeharbor/square": ({ 30000025, "settlement" }),
      "place/freeharbor/market": ({ 30000026, "settlement" }),
      "place/freeharbor/hall": ({ 30000027, "interior" }),
      "place/freeharbor/hearth": ({ 30000028, "interior" }),
      "place/freeharbor/yard": ({ 30000029, "settlement" })
    ]),
    "routes": ([
      "freeharbor-keepingstead": ([
        "name": "South Coast Loop",
        "mode": "ferry",
        "from": "place/freeharbor/arrival",
        "to": "place/keepingstead/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "The charter crew returns a load of empty salt barrels, making way for helpers "
            + "from the next port."
      ]),
      "freeharbor-ironhold": ([
        "name": "Old Company Train",
        "mode": "caravan",
        "from": "place/freeharbor/arrival",
        "to": "place/ironhold/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "The route passes wet pasture into steep wooded country. One of the drivers gives "
            + "a spare blanket to a passenger who came off the ferry without a coat."
      ]),
      "freeharbor-havenrode": ([
        "name": "West–South Coastal Packet",
        "mode": "ferry",
        "from": "place/freeharbor/arrival",
        "to": "place/havenrode/arrival",
        "duration": 120,
        "fare": 0,
        "description":
            "The ship runs through swells toward a line of guest houses lit behind the quay."
      ]),
      "freeharbor-driftwood": ([
        "name": "Salvagers' Run",
        "mode": "ferry",
        "from": "place/freeharbor/arrival",
        "to": "place/driftwood/arrival",
        "duration": 80,
        "fare": 0,
        "description":
            "The tender follows a trail of carved warning posts to the place where Driftwood "
            + "crews turn over the tide."
      ]),
      "freeharbor-grey-step": ([
        "name": "Grey Relay",
        "mode": "coach",
        "from": "place/freeharbor/arrival",
        "to": "place/grey-step/arrival",
        "duration": 65,
        "fare": 0,
        "description":
            "The driver hands off a sealed length of sailcloth at a relay post where there is "
            + "no garrison guard."
      ]),
      "freeharbor-moorcross": ([
        "name": "Moor Harness Coach",
        "mode": "coach",
        "from": "place/freeharbor/arrival",
        "to": "place/moorcross/arrival",
        "duration": 70,
        "fare": 0,
        "description":
            "Wind drives rain across the road. The driver uses a wool sheet to keep the lamp "
            + "from going out."
      ])
    ]),
    "trails": ([
      "freeharbor-tide-chapel": ([
        "from": "freeharbor",
        "to": "tide-chapel",
        "steps": ({ "west", "west", "west" }),
        "description":
            "A red pennant warns travelers to stay clear of a stretch of tide-dark stone. The "
            + "old meeting shelter opens north into the hillside."
      ]),
      "freeharbor-press-yard": ([
        "from": "freeharbor",
        "to": "press-yard",
        "steps": ({ "north", "north" }),
        "description":
            "A flight of shallow steps carries cuts intended to trip a loaded cart in wet "
            + "weather. The walls bear pitch at shoulder height where the old gate shuddered "
            + "into place."
      ])
    ]),
    "landmarks": ([ ])
  ]);
}
