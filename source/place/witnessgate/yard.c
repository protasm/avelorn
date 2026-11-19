inherit "system/place";

mapping definition() {
  return ([
    "name": "Bell Loft",
    "description":
        "The upper landing smells of hemp and hot [brass]. Bell keepers teach the watch to "
        + "count beats between calls and to move when words are likely to be misunderstood.",
    "details": ([
      "brass":
          "The bell has one clean note and a shorter dull one from a hairline crack. New "
          + "keepers practice repeating a call so the two are never confused."
    ]),
    "environment": "settlement",
    "exits": ([
      "down": "place/witnessgate/square"
    ]),
    "services": ({ "training" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
