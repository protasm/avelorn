inherit "system/place";

mapping definition() {
  return ([
    "name": "Holt Coach Stop",
    "description":
        "Coach wheels leave deep radial cuts around a turning post in the forest. A [slate] "
        + "records the opening hour of the road every day since the levy began.",
    "details": ([
      "slate":
          "An old time remains visible under a smeared new one. Use `note time` while the coach "
          + "is still turning for an independent record."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2282/y4711",
      "north": "place/beacon-holt/hearth",
      "east": "place/beacon-holt/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "before-bell-one-note",
        "verb": "note",
        "argument": "time",
        "text": "The coach has begun turning while the slate still shows yesterday's entry."
      ])
    })
  ]);
}
