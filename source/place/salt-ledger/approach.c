inherit "system/place";

mapping definition() {
  return ([
    "name": "Salt Ledger Lane",
    "description":
        "Fish [hooks] hang from a broken boundary line. The walk north leads to a "
        + "counting-house built on the site of a communal meal shed.",
    "details": ([
      "hooks":
          "The hooks once held fish for travelers to take freely. New lead tags on the ends "
          + "record a price and the name of the counting-house."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x0809/y3756",
      "north": "place/salt-ledger/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
