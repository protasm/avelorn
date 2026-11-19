inherit "system/place";

mapping definition() {
  return ([
    "name": "Freeharbor Slip",
    "description":
        "Cliff steps switch back between pale pilings and a line of houses above the surf. Each "
        + "pilot flies a different [flag]. The square is north. The path out leads three squares "
        + "west to Tide Chapel.",
    "details": ([
      "flag":
          "Between the commission colors, one bare line is kept for a signal any pilot can "
          + "answer. Use `raise storm-mark` for the mooring or `count crews` for the voluntary "
          + "watch."
    ]),
    "environment": "settlement",
    "exits": ([
      "north": "place/freeharbor/square",
      "out": "place/atlas/x2356/y3170"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-third-mooring-raise",
        "verb": "raise",
        "argument": "storm-mark",
        "text":
            "The plain strip flies where a pilot can see it without mistaking it for any "
            + "commission."
      ]),
      ([
        "tag": "the-voluntary-watch-count",
        "verb": "count",
        "argument": "crews",
        "text":
            "There are enough freely promised crews for a storm watch and several people "
            + "offering dry blankets ashore."
      ])
    })
  ]);
}
