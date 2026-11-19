inherit "system/place";

mapping definition() {
  return ([
    "name": "Saltbreak Mooring",
    "description":
        "The landing reached its present height three storms ago, as shown by old [rings] "
        + "halfway down a rebuilt wall. The ferry buffers are rolls of knotted net.",
    "details": ([
      "rings":
          "Each discarded iron ring is set into a different course of stone. Use `read "
          + "tide-ring` to work out how high the next brace will have to sit."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x3326/y3454",
      "north": "place/saltbreak/hearth",
      "east": "place/saltbreak/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-far-wall-read",
        "verb": "read",
        "argument": "tide-ring",
        "text": "The old ring sits below this year's tide line. The new braces have to sit higher."
      ])
    })
  ]);
}
