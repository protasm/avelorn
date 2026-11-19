inherit "system/place";

mapping definition() {
  return ([
    "name": "Hollow Market",
    "description":
        "Cider skeins and [grafting blades] hang under a slate roof. An orchard owner covers a "
        + "crate of bruised fruit before an inspector can count it at full value.",
    "details": ([
      "grafting blades":
          "Colored cuts in the handle ends match knots on a wrapped root bundle. Use `check "
          + "grafts` against Lena's list of plots."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/orchard-cross/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "action": ([
      "tag": "check-orchard-grafts",
      "verb": "check",
      "argument": "grafts",
      "text":
          "The missing graft numbers match plots south of the new ditch. You note the "
          + "discrepancy for Lena."
    ])
  ]);
}
