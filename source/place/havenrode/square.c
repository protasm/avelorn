inherit "system/place";

mapping definition() {
  return ([
    "name": "Hearth Row",
    "description":
        "Open doors face a communal bread oven. Blue [tiles] show households that still have "
        + "space for guests. A Dominion assessor has nailed a blank brass plate over his door "
        + "rather than claim any family.",
    "details": ([
      "tiles":
          "A blue tile with a single notch offers a bed. Two notches promise space for a "
          + "family. On one lintel the older notches have been carefully widened."
    ]),
    "environment": "settlement",
    "exits": ([
      "south": "place/havenrode/arrival",
      "east": "place/havenrode/market",
      "north": "place/havenrode/hall",
      "west": "place/havenrode/hearth",
      "up": "place/havenrode/yard"
    ]),
    "services": ({ "noticeboard" }),
    "npcs": ({ }),
    "safe": 0,
    "quests": ({ "a-nail-without-a-name" })
  ]);
}
