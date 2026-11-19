inherit "system/place";

mapping definition() {
  return ([
    "name": "Driftwood Steps",
    "description":
        "The road descends in sharp cuts from the timber terrace. Porters roll the ferry's "
        + "bundles inland on old [mast spars] without setting foot in the shingle.",
    "details": ([
      "mast spars":
          "Several spars still carry mast marks recognized by old fishing crews. Use `invite "
          + "claimant` at the steps when the salvagers' article is ready."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2037/y3509",
      "north": "place/driftwood/hearth",
      "east": "place/driftwood/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "a-charter-in-scraps-invite",
        "verb": "invite",
        "argument": "claimant",
        "text":
            "A porter's family comes up from the steps carrying the pulley promised under the "
            + "old terms."
      ])
    })
  ]);
}
