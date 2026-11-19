inherit "system/place";

mapping definition() {
  return ([
    "name": "Resin Table",
    "description":
        "Little bundles of pitch wood coat a seller's fingers. A tinsmith displays a surviving "
        + "red [shutter] from a lantern called blind in the official log.",
    "details": ([
      "shutter":
          "The red tin throws a clear beam only when the wick is trimmed even. For the timing "
          + "inquiry, use `trim wick` before the watch arrives."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/beacon-holt/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "before-bell-one-trim",
        "verb": "trim",
        "argument": "wick",
        "text": "You bring the lamp flame even before the regular watch arrives."
      ])
    })
  ]);
}
