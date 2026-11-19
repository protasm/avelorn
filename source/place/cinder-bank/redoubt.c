inherit "system/place";

mapping definition() {
  return ([
    "name": "Quarry Redoubt",
    "description":
        "A shale shelf conceals the alternate exit. From below, a [rope] resting against the "
        + "rock would look like a narrow shadow. A scout listens for boots on the scree.",
    "details": ([
      "rope":
          "Its release knot sits a little inside the shelf. Use `inspect descent-rope` to make "
          + "sure a departing person can reach it without leaning over the scree."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/cinder-bank/forge"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "three-voices-at-cinder-bank-inspect",
        "verb": "inspect",
        "argument": "descent-rope",
        "text":
            "The rope has moved once since the patrol bell rang. You check that the release "
            + "knot remains reachable from inside."
      ])
    })
  ]);
}
