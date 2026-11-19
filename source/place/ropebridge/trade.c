inherit "system/place";

mapping definition() {
  return ([
    "name": "Splicers' Walk",
    "description":
        "A tensioned line ends at a spindle set into an enormous stone. Beside the spindle, a "
        + "[basket] of sound rope ends is set out for travelers who cannot pay.",
    "details": ([
      "basket":
          "Sound lengths sit apart from damaged ones, sorted to the reach of a traveler. Use "
          + "`splice rope` with Bertan's task or `stow spare-line` for the storm count."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/ropebridge/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "action": ([
      "tag": "splice-bridge-rope",
      "verb": "splice",
      "argument": "rope",
      "text":
          "Under the runner's eye you splice sound rope to replace a worn strand and leave "
          + "the old end marked for inspection."
    ]),
    "actions": ({
      ([
        "tag": "someone-holds-the-back-stow",
        "verb": "stow",
        "argument": "spare-line",
        "text":
            "You set a traveler-length rope clear of the tool basket and count it out to the "
            + "next runner."
      ])
    })
  ]);
}
