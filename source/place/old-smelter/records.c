inherit "system/place";

mapping definition() {
  return ([
    "name": "Shift Roster",
    "description":
        "A [slate] at the rear records food shares paid to six crews while they rebuilt the "
        + "pooled furnace. The unpaid crew has the oldest chalk.",
    "details": ([
      "slate":
          "Chalk names several workers who never received food shares. Use `count hands` before "
          + "the companies compare the tally at Crew Hearth."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/old-smelter/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-companions-count-count",
        "verb": "count",
        "argument": "hands",
        "text":
            "You count people and dependents across the workers' slates before the foundry "
            + "takes its total."
      ])
    })
  ]);
}
