inherit "system/place";

mapping definition() {
  return ([
    "name": "Whitequay Pier",
    "description":
        "Long sheds on half-submerged stone feet wait for cargo beyond the spring tide. The "
        + "chalk [arrival board] bears two signatures for every name.",
    "details": ([
      "arrival board":
          "There is room for a second signature beneath the old measure, but no chalk in the "
          + "groove. Use `study board` for the weighing dispute."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2395/y4685",
      "north": "place/whitequay/hearth",
      "east": "place/whitequay/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-whitequay-weighing-study",
        "verb": "study",
        "argument": "board",
        "text": "The customs board records that the old measure never received a second signature."
      ])
    })
  ]);
}
