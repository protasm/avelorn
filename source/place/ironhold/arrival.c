inherit "system/place";

mapping definition() {
  return ([
    "name": "Ironhold Cart Gate",
    "description":
        "A [weighhouse] stands between the mining road and the first inhabited terrace. The "
        + "pack-train lead takes any passenger who can wrap their own boots. The square is north. "
        + "Out and east three squares lies the Empty Winch.",
    "details": ([
      "weighhouse":
          "A tally line below the loading ramp counts passengers as well as ore. The train lead "
          + "keeps a spare pair of dry wrappings beside the scale."
    ]),
    "environment": "settlement",
    "exits": ([
      "north": "place/ironhold/square",
      "out": "place/atlas/x2320/y3800"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
