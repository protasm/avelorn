inherit "system/place";

mapping definition() {
  return ([
    "name": "Weir Workers' Hall",
    "description":
        "The [shift schedule] hangs from a wooden peg between two towels. Anybody stepping in "
        + "for a sick neighbor puts their own name beside the missing one so both families get "
        + "credit.",
    "details": ([
      "shift schedule":
          "The hand that wrote a substitute's name has written the sick worker's name again "
          + "underneath it. The double entry earns pay for both instead of striking either worker "
          + "off."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/eastweir/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "the-older-repair-ledger" })
  ]);
}
