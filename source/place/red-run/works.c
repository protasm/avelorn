inherit "system/place";

mapping definition() {
  return ([
    "name": "Spillway Foot",
    "description":
        "Brush piles dam one of the smaller [drains]. In front of the obstruction, teeth have "
        + "eaten notches in the spare piles of oak stakes.",
    "details": ([
      "drains":
          "The smaller gate has a hand-cut notch for gauging flow with a single stick. Use "
          + "`gauge drain` before posting the weekly reading."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/red-run/approach",
      "down": "place/red-run/records"
    ]),
    "services": ({ }),
    "npcs": ({ "kilnback-boar" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "first-water-round-gauge",
        "verb": "gauge",
        "argument": "drain",
        "text":
            "The red drain has settled below the danger mark but the smaller gate can use a "
            + "fresh brace."
      ])
    })
  ]);
}
