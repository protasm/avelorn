inherit "system/place";

mapping definition() {
  return ([
    "name": "Tile Track",
    "description":
        "The low ground smells of alder bark and strong pipe clay. The carriage stop shares a "
        + "high [stone pad] with a wagon waiting for a fresh wheel.",
    "details": ([
      "stone pad":
          "A water stain turns at the pad where an earlier gate failed. Use `read waterline` to "
          + "compare it with the fitter's saved peg."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2802/y3702",
      "north": "place/eastweir/hearth",
      "east": "place/eastweir/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-older-repair-ledger-read",
        "verb": "read",
        "argument": "waterline",
        "text":
            "The strain line reaches an abandoned peg hole a finger-width below the current "
            + "hinge."
      ])
    })
  ]);
}
