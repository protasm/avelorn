inherit "system/place";

mapping definition() {
  return ([
    "name": "Gang Yard",
    "description":
        "Unused [hooks] are threaded with red levy cord. A few are bent double. Behind them a "
        + "tally of seized goods was washed off so many times it stained the brick.",
    "details": ([
      "hooks":
          "The threads on the hooks were once tied to bundles before their owners were taken. "
          + "An intact knot still holds the tag of a boat seized for the levy."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/press-yard/approach",
      "down": "place/press-yard/records"
    ]),
    "services": ({ }),
    "npcs": ({ "press-captain" }),
    "safe": 0
  ]);
}
