inherit "system/place";

mapping definition() {
  return ([
    "name": "Cold Mold Floor",
    "description":
        "Unused [clay molds] lie in rows with names set into their open faces. A squat suit of "
        + "black iron covers the inlet between the rows.",
    "details": ([
      "clay molds":
          "Each mold carries a crew name backwards so it would come out legible on the fitting. "
          + "Some have been turned over to hide the names from the foundry guard."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/old-smelter/approach",
      "down": "place/old-smelter/records"
    ]),
    "services": ({ }),
    "npcs": ({ "mold-sentinel" }),
    "safe": 0
  ]);
}
