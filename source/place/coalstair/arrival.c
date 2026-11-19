inherit "system/place";

mapping definition() {
  return ([
    "name": "Coalstair Saddle",
    "description":
        "The mountain shows its bare flank above a low line of [cabins]. Pack train passengers "
        + "wait for the air to settle. Cairns mark the old vault approach, seventeen wilderness "
        + "squares east and then six south.",
    "details": ([
      "cabins":
          "A plain flag lies folded by the roof of the nearest cabin. Use `hang kettle-flag` "
          + "after reading the shift token so returning guides can spot shelter."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2395/y3726",
      "north": "place/coalstair/hearth",
      "east": "place/coalstair/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "under-an-old-wage-hang",
        "verb": "hang",
        "argument": "kettle-flag",
        "text":
            "You hang the old signal so returning guides know they can shelter and settle "
            + "arrears here."
      ])
    })
  ]);
}
