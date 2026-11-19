inherit "system/place";

mapping definition() {
  return ([
    "name": "Rope Square",
    "description":
        "Little groups stand around whitewashed [stones] while runners carry the day's "
        + "proposals from one corner to another. There is a clear lane in all directions so "
        + "walking away is always possible.",
    "details": ([
      "stones":
          "There is room on the proposal stone to leave a name or pass in silence. Use `invite "
          + "volunteers` here when the watch is ready to find its crews."
    ]),
    "environment": "settlement",
    "exits": ([
      "south": "place/freeharbor/arrival",
      "east": "place/freeharbor/market",
      "north": "place/freeharbor/hall",
      "west": "place/freeharbor/hearth",
      "up": "place/freeharbor/yard"
    ]),
    "services": ({ "noticeboard" }),
    "npcs": ({ }),
    "safe": 0,
    "quests": ({ "the-chair-by-the-door" }),
    "actions": ({
      ([
        "tag": "the-voluntary-watch-invite",
        "verb": "invite",
        "argument": "volunteers",
        "text":
            "The sheet fills in several hands. Spaces remain deliberately open for people who "
            + "want to decline."
      ])
    })
  ]);
}
