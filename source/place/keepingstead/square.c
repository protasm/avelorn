inherit "system/place";

mapping definition() {
  return ([
    "name": "Measure Square",
    "description":
        "A cobbled gutter carries rain between two bricked granaries. Each building has a clean "
        + "slot where its [repair ledger] can be read with wet hands. Behind the posts, the "
        + "Dominion quota has been repainted twice.",
    "details": ([
      "repair ledger":
          "All the repair notes name both a worker and the household that gets a dry doorstep "
          + "once the work is done. A clean page waits beneath the new levy."
    ]),
    "environment": "settlement",
    "exits": ([
      "south": "place/keepingstead/arrival",
      "east": "place/keepingstead/market",
      "north": "place/keepingstead/hall",
      "west": "place/keepingstead/hearth",
      "up": "place/keepingstead/yard"
    ]),
    "services": ({ "noticeboard" }),
    "npcs": ({ }),
    "safe": 0,
    "quests": ({ "dry-sacks" })
  ]);
}
