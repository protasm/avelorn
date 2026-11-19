inherit "system/place";

mapping definition() {
  return ([
    "name": "Drift Table",
    "description":
        "A storm split the original table down the middle. Crews meet here without a chair at "
        + "either end, and the day's refused contracts are left on a [second table].",
    "details": ([
      "second table":
          "Folded pages on this table are left unsigned to show that declining a contract need "
          + "not be an argument. One entire crew has refused to sell its oars this week."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/driftwood/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ "tamsin-rowe" }),
    "safe": 1,
    "quests": ({ "a-charter-in-scraps" })
  ]);
}
