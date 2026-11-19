inherit "system/place";

mapping definition() {
  return ([
    "name": "Free Table",
    "description":
        "The benches here were moved from a closed inn by six households. A cook with a torn "
        + "sleeve stirs oat stew and keeps a [spare bowl] ready for anyone turned out of the "
        + "assembly after dark.",
    "details": ([
      "spare bowl":
          "A glazier repaired the bowl with five brass staples. The six households agreed to "
          + "share its care, so it stays ready whoever is cooking."
    ]),
    "environment": "interior",
    "exits": ([
      "east": "place/freeharbor/square"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "the-third-mooring" })
  ]);
}
