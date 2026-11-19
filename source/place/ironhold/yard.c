inherit "system/place";

mapping definition() {
  return ([
    "name": "Inspection Drill",
    "description":
        "A track of fixed [pins] runs up the wall. Retired foremen teach new crews to carry "
        + "equipment under load and take the back of a rope when a companion stumbles.",
    "details": ([
      "pins":
          "Every fourth pin is set near a shallow depression for an injured worker's foot. A "
          + "crew can keep climbing while sharing that person's load."
    ]),
    "environment": "settlement",
    "exits": ([
      "down": "place/ironhold/square"
    ]),
    "services": ({ "training" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
