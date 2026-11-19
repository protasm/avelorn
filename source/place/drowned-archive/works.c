inherit "system/place";

mapping definition() {
  return ([
    "name": "Submerged Index",
    "description":
        "Ink-stained reeds grow on the landings. Talon marks scar the [racks] where archivists "
        + "used to hang rolled testimony.",
    "details": ([
      "racks":
          "Several roll labels still hang by lengths of soaked thread. Long dark feathers are "
          + "stuck between the next two pegs."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/drowned-archive/approach",
      "down": "place/drowned-archive/records"
    ]),
    "services": ({ }),
    "npcs": ({ "inkback-heron" }),
    "safe": 0
  ]);
}
