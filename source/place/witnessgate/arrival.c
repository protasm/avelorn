inherit "system/place";

mapping definition() {
  return ([
    "name": "Witnessgate Wharf",
    "description":
        "Carved [posts] record the names of two witnesses for each pier. A clerk reads cargo "
        + "declarations across the water so there can be no objection that nobody heard. The "
        + "square is north. Out and three squares west is the old Record Vault.",
    "details": ([
      "posts":
          "Pier builders carved names in pairs so a missing witness could not be filed as a "
          + "missing pier. One post carries a second pair low enough to escape a clerk's glance."
    ]),
    "environment": "settlement",
    "exits": ([
      "north": "place/witnessgate/square",
      "out": "place/atlas/x2520/y4377"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
