inherit "system/place";

mapping definition() {
  return ([
    "name": "Anchor Room",
    "description":
        "The night crew recently moved the stove closer to the [rope wall]. A woman winds used "
        + "cord for the apprentices and tells them which kinds of wear can never be trusted.",
    "details": ([
      "rope wall":
          "Pieces of cord from old bridges hang where apprentices can handle them. The fibers "
          + "that are least safe sometimes show the fewest breaks."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/ropebridge/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ "bertan-lent" }),
    "safe": 1,
    "quests": ({ "someone-holds-the-back" })
  ]);
}
