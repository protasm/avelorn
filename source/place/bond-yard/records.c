inherit "system/place";

mapping definition() {
  return ([
    "name": "Guest Bench",
    "description":
        "A broad [bench] has been moved from a hall to this locked office. A board above it "
        + "still names the families who sent it.",
    "details": ([
      "bench":
          "An apprentice carved five cups into the underside. The families named on the board "
          + "used to lend this bench for public meals."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/bond-yard/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
