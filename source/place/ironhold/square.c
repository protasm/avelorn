inherit "system/place";

mapping definition() {
  return ([
    "name": "Company Square",
    "description":
        "Iron [pipes] warm the cobbles. A line of widows waits beside the pay office for shares "
        + "owed by the dead to the living. Across the square, the foundry guard ignores them.",
    "details": ([
      "pipes":
          "The warm pipes end at the pay office, long before they reach the older houses. "
          + "People waiting for the dead crews' shares stand along them in the morning."
    ]),
    "environment": "settlement",
    "exits": ([
      "south": "place/ironhold/arrival",
      "east": "place/ironhold/market",
      "north": "place/ironhold/hall",
      "west": "place/ironhold/hearth",
      "up": "place/ironhold/yard"
    ]),
    "services": ({ "noticeboard" }),
    "npcs": ({ }),
    "safe": 0,
    "quests": ({ "company-wages" })
  ]);
}
