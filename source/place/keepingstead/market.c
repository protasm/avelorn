inherit "system/place";

mapping definition() {
  return ([
    "name": "Good Order Market",
    "description":
        "Orchard runners sell long-keeping apples in straw twists. A bowl of rejected [nails] "
        + "sits on a tiler's stall, each marked with the batch that made it too soft.",
    "details": ([
      "nails":
          "Each nail is bent to the same angle and tagged with its firing date. A dyer's "
          + "apprentice comes by to buy the rejected ones for hanging cloth."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/keepingstead/square"
    ]),
    "services": ({ "market", "noticeboard" }),
    "npcs": ({ "nara-crook" }),
    "safe": 0,
    "quests": ({ "first-water-round" })
  ]);
}
