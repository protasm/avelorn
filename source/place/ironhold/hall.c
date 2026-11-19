inherit "system/place";

mapping definition() {
  return ([
    "name": "Old Company Hall",
    "description":
        "Knots on a [memorial rope] show where comrades were lost. Every company once brought "
        + "its own rope here for a public inspection. New ropes bear the crest of a ministry and "
        + "have no knots.",
    "details": ([
      "memorial rope":
          "A second knot below some of the older knots marks a casualty reported late. Use "
          + "`trace memorial-rope` to compare these names with the ministry roster."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/ironhold/square"
    ]),
    "services": ({ }),
    "npcs": ({ "torren-mattock" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "company-wages-trace",
        "verb": "trace",
        "argument": "memorial-rope",
        "text": "You count knots for people who died after the payroll marked them as transferred."
      ])
    })
  ]);
}
