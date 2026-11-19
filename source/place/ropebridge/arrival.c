inherit "system/place";

mapping definition() {
  return ([
    "name": "Ropebridge Approach",
    "description":
        "Guide staffs are racked point down where the packed track meets a cleft. Beside each "
        + "staff hangs a [cord] marked for the number of travelers it can bring back.",
    "details": ([
      "cord":
          "Little travel knots count everyone assigned a place on the span. Use `count cords` "
          + "to check that someone will also hold the back of the line."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2140/y3860",
      "north": "place/ropebridge/hearth",
      "east": "place/ropebridge/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "someone-holds-the-back-count",
        "verb": "count",
        "argument": "cords",
        "text":
            "One staff is not marked for the person holding the back of the line. You add a "
            + "separate cord for that duty."
      ])
    })
  ]);
}
