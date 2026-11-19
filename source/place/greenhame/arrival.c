inherit "system/place";

mapping definition() {
  return ([
    "name": "Dune Gate",
    "description":
        "Half-buried fencing corrals shaggy cattle above the dunes. The coach stops beside a "
        + "low [post] showing which households have offered spare beds.",
    "details": ([
      "post":
          "A deep notch marks a house with a spare bed. A shallow one means a meal only. A "
          + "foster child has scratched a third shape that nobody has painted over."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x0350/y3542",
      "north": "place/greenhame/hearth",
      "east": "place/greenhame/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
