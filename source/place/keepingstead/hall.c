inherit "system/place";

mapping definition() {
  return ([
    "name": "Seaward Keeping-House",
    "description":
        "A table map shows roads, spillways, storehouses, and ovens. Two [maps] hang behind it. "
        + "One marks stock owed to the Dominion and the other marks households at immediate risk "
        + "of going hungry.",
    "details": ([
      "maps":
          "The quota map counts stocks by granary, while the household map counts meals needed "
          + "before morning. Use `mark ration-map` for the low stores or `post reading` when "
          + "surveying Red Run."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/keepingstead/square"
    ]),
    "services": ({ }),
    "npcs": ({ "aften-vale" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "dry-sacks-mark",
        "verb": "mark",
        "argument": "ration-map",
        "text":
            "The low households get their own marks instead of disappearing under the "
            + "garrison's total."
      ]),
      ([
        "tag": "first-water-round-post",
        "verb": "post",
        "argument": "reading",
        "text":
            "You post the new drain reading where the market and the low reserves can both "
            + "claim it."
      ])
    })
  ]);
}
