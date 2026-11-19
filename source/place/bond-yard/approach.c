inherit "system/place";

mapping definition() {
  return ([
    "name": "Bond Yard Lane",
    "description":
        "Iron spear rests stand outside the loan house. A shuttered [window] carries the old "
        + "symbol for an empty bed offered to a stranger.",
    "details": ([
      "window":
          "The painted bed above the shutter has a short line beneath it, once the sign for "
          + "fresh blankets. A nail now holds the loan-house's fees over the line."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x0812/y3754",
      "north": "place/bond-yard/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
