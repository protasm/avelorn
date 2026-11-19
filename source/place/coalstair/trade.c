inherit "system/place";

mapping definition() {
  return ([
    "name": "Tether Rail",
    "description":
        "Thick ropes loop over carved [stone horns]. Pack animals lean away from the higher "
        + "trail, where the wind occasionally carries a sound like several voices speaking "
        + "perfectly together.",
    "details": ([
      "stone horns":
          "A stamped brass token is lashed to one horn beneath the old crew's knot. Use `read "
          + "shift-token` before raising the flag at the saddle."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/coalstair/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "under-an-old-wage-read",
        "verb": "read",
        "argument": "shift-token",
        "text":
            "The worn code belongs to one of the old licensed crews, paid by the haul rather "
            + "than the day."
      ])
    })
  ]);
}
