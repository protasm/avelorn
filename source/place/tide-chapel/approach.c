inherit "system/place";

mapping definition() {
  return ([
    "name": "Bent Sign Path",
    "description":
        "A red [pennant] warns travelers to stay clear of a stretch of tide-dark stone. The old "
        + "meeting shelter opens north into the hillside.",
    "details": ([
      "pennant":
          "Its frayed end points inland though the wind comes off the water. A sailor weighted "
          + "that edge to warn of the steps below even in still air."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x2353/y3170",
      "north": "place/tide-chapel/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
