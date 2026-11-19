inherit "system/place";

mapping definition() {
  return ([
    "name": "Grey Step Track",
    "description":
        "Cart grooves lead out between rows of short tide grass. The sand drifts against the "
        + "inner sides of low grey [walls], keeping the road passable for the post.",
    "details": ([
      "walls":
          "An exposed section of wall serves as a levy record. Between two accepted shifts it "
          + "keeps a clear gap. Use `attest blank` to witness the refusal."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2171/y3393",
      "north": "place/grey-step/hearth",
      "east": "place/grey-step/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "fifth-crew-refusal-attest",
        "verb": "attest",
        "argument": "blank",
        "text": "You leave the refusal space blank and put a dated witness mark beside it."
      ])
    })
  ]);
}
