inherit "system/place";

mapping definition() {
  return ([
    "name": "Breakwater Yard",
    "description":
        "Young pilots practice hauling weights in pairs. A [chalk line] marks the reach of a "
        + "boarding pike and keeps trainees clear of the work crews.",
    "details": ([
      "chalk line":
          "Someone has redrawn the line inward so tired pilots can finish their drill. Use `ask "
          + "crew` here about how much tar the breakwater work used."
    ]),
    "environment": "settlement",
    "exits": ([
      "down": "place/havenrode/square"
    ]),
    "services": ({ "training" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "tarbills-ask",
        "verb": "ask",
        "argument": "crew",
        "text": "The breakwater crew confirms there was only enough tar for one coat."
      ])
    })
  ]);
}
