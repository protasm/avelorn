inherit "system/place";

mapping definition() {
  return ([
    "name": "Axle Shed",
    "description":
        "A wright rolls a cold [wheel hoop] onto the brick floor. A seller weighs honey against "
        + "dry oats to fill the first order for the inland coach.",
    "details": ([
      "wheel hoop":
          "The wright leaves a short stretch of the hoop bare of paint so a driver can spot a "
          + "new crack without pulling the wheel off."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/greenhame/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
