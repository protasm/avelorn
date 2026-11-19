inherit "system/place";

mapping definition() {
  return ([
    "name": "Ironhold Exchange",
    "description":
        "Rope makers count slack against fixed iron eyes. Ore factors argue through a grated "
        + "window as ore dust collects in the corners of a [silver scale].",
    "details": ([
      "silver scale":
          "A factor weighs ore in its left pan and sets papers for wage claims in its right. "
          + "Use `file claim` with the memorial and pay evidence."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/ironhold/square"
    ]),
    "services": ({ "market", "noticeboard" }),
    "npcs": ({ "adel-kern" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "company-wages-file",
        "verb": "file",
        "argument": "claim",
        "text":
            "The claim names both debts and puts survival provisions before the issue of "
            + "interest."
      ])
    }),
    "quests": ({ "the-companions-count" })
  ]);
}
