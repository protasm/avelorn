inherit "system/place";

mapping definition() {
  return ([
    "name": "Silt House Bank",
    "description":
        "An embankment path rises north to a timber sash gate. The ditch beside it leaves a "
        + "heavy [clay mark] above the usual water line.",
    "details": ([
      "clay mark":
          "The mark ends well above the usual silting line. A loose reed remains trapped in it, "
          + "stem pointing back toward the low granary."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x3030/y3856",
      "north": "place/silt-house/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
