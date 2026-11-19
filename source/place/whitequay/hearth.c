inherit "system/place";

mapping definition() {
  return ([
    "name": "Freight Hostel",
    "description":
        "A [card] over each bunk states the price, the hour it is owed, and a name to quote if "
        + "someone asks for more. A quiet table is reserved for travelers still deciding where to "
        + "go.",
    "details": ([
      "card":
          "A low price has been crossed out and written back in. Below it the hostel keeper "
          + "offers to stand witness to the original rate."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/whitequay/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "the-whitequay-weighing" })
  ]);
}
