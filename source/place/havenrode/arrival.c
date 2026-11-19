inherit "system/place";

mapping definition() {
  return ([
    "name": "Havenrode Quay",
    "description":
        "Stone [steps] climb from a curved quay into a stand of sea-firs. A ferryman pours the "
        + "first cup of the day onto the stones for travelers who never arrived. The square is "
        + "north. The track out leads toward Salt Ledger House, three squares west.",
    "details": ([
      "steps":
          "Little shells are packed beneath one tread, where a stone has begun to lift. For the "
          + "weekly quay inspection, use `check stair`."
    ]),
    "environment": "settlement",
    "exits": ([
      "north": "place/havenrode/square",
      "out": "place/atlas/x0812/y3756"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "shells-beneath-the-quay-check",
        "verb": "check",
        "argument": "stair",
        "text":
            "A shell line shows fresh movement under the third step. You leave a practical "
            + "warning mark."
      ])
    })
  ]);
}
