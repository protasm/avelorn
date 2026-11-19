inherit "system/place";

mapping definition() {
  return ([
    "name": "Keel Market",
    "description":
        "Salt fish dries over a roof of tarred canvas. The first stalls sell small arms, "
        + "[cordage], and clean wool. A repair crew at the last stall haggles over whether an "
        + "emergency commission carries a right to refuse.",
    "details": ([
      "cordage":
          "Scraps of cloth are tied to cord ends by their owners. Use `collect flags` for the "
          + "storm mooring pattern."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/freeharbor/square"
    ]),
    "services": ({ "market", "noticeboard" }),
    "npcs": ({ "pol-eld" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-third-mooring-collect",
        "verb": "collect",
        "argument": "flags",
        "text":
            "Union flags, house flags, and a plain white strip can each make the missing "
            + "pattern."
      ])
    }),
    "quests": ({ "the-voluntary-watch" })
  ]);
}
