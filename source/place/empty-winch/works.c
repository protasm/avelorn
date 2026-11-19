inherit "system/place";

mapping definition() {
  return ([
    "name": "Winch Floor",
    "description":
        "The floor is warm where a loaded rope passes down a bore. Under the lip of the shaft, "
        + "the bright steel [catching teeth] flex before the rope moves.",
    "details": ([
      "catching teeth":
          "A stable lamp burns above the teeth, which close before the rope strikes them. Use "
          + "`shade lamp` to see whether another light answers below."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/empty-winch/approach",
      "down": "place/empty-winch/records"
    ]),
    "services": ({ }),
    "npcs": ({ "bonded-overseer" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "a-light-below-the-winch-shade",
        "verb": "shade",
        "argument": "lamp",
        "text":
            "Light falls through a path left between the catches. Someone below could signal "
            + "through it."
      ])
    })
  ]);
}
