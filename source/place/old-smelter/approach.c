inherit "system/place";

mapping definition() {
  return ([
    "name": "Old Smelter Grade",
    "description":
        "The path bears slips where a metal runner spun out with an ore pan. In the past year "
        + "someone drove [guide pins] deep into the stone.",
    "details": ([
      "guide pins":
          "Heads of older pins survive below the new ones. A loaded worker can follow the lower "
          + "row without lifting an ore pan off their shoulder."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x2320/y3802",
      "north": "place/old-smelter/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
