inherit "system/place";

mapping definition() {
  return ([
    "name": "Press Yard Stairs",
    "description":
        "A flight of shallow [steps] carries cuts intended to trip a loaded cart in wet "
        + "weather. The walls bear pitch at shoulder height where the old gate shuddered into "
        + "place.",
    "details": ([
      "steps":
          "The old cuts are deep enough to jam a loaded wheel. Footprints stay close to the "
          + "wall where a prisoner's rope once ran."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x2356/y3172",
      "north": "place/press-yard/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
