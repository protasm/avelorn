inherit "system/place";

mapping definition() {
  return ([
    "name": "Stone-Line Yard",
    "description":
        "A [teaching rope] divides a stretch of limestone. Recruits practice holding a line, "
        + "breaking it, and coming back together without taking an officer's order.",
    "details": ([
      "teaching rope":
          "Two knots mark the places a recruit can withdraw from the line. The rest practice "
          + "closing the gap without claiming a right to call the absent person back."
    ]),
    "environment": "settlement",
    "exits": ([
      "down": "place/freeharbor/square"
    ]),
    "services": ({ "training" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
