inherit "system/place";

mapping definition() {
  return ([
    "name": "Sluice Chamber",
    "description":
        "A counterweight has been wedged above a channel that ought to be running freely. Cut "
        + "tally marks on the [lever] show which gate feeds each neighborhood.",
    "details": ([
      "lever":
          "Wear around the lowest grip leaves room for a second hand from the opposite side. "
          + "Use `chalk key` to mark where helpers should stand for a lift."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/silt-house/approach",
      "down": "place/silt-house/records"
    ]),
    "services": ({ }),
    "npcs": ({ "ration-sergeant" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "weir-keys-chalk",
        "verb": "chalk",
        "argument": "key",
        "text":
            "On the chamber wall you mark where a second helper must stand before the gate is "
            + "raised."
      ])
    })
  ]);
}
