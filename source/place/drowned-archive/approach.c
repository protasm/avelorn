inherit "system/place";

mapping definition() {
  return ([
    "name": "Drowned Archive Towpath",
    "description":
        "Flood readings are cut into a wind-smoothed [beam]. The lowest doors show where the "
        + "most recent tide stopped.",
    "details": ([
      "beam":
          "The lowest row of numbers dates back to the independent court. The latest flood has "
          + "left a strip of reed pulp over all but the top figure."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x2520/y4375",
      "north": "place/drowned-archive/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
