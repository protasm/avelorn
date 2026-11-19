inherit "system/place";

mapping definition() {
  return ([
    "name": "Headland Lamp Walk",
    "description":
        "The lamp post is scratched halfway up, and a broken fish [basket] lies beneath it. The "
        + "flat stones toward the signal dome still carry a line of white paint.",
    "details": ([
      "basket":
          "The weave was broken outward from the rim. A trail of fish scales runs from it to "
          + "the wolf's prints on the lamp path."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x0114/y3336",
      "east": "place/headland-signal/signal-dome"
    ]),
    "services": ({ }),
    "npcs": ({ "headland-wolf" }),
    "safe": 0
  ]);
}
