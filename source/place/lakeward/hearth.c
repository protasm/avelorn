inherit "system/place";

mapping definition() {
  return ([
    "name": "Covered Hearth",
    "description":
        "Boiled fish and toasted grain steam on a table dragged close to an open shutter. Each "
        + "newcomer is given a piece of [charcoal] to add to a low perpetual fire.",
    "details": ([
      "charcoal":
          "Fresh charcoal is tucked into hollows left by other guests. After a successful "
          + "rescue, the hearth keeper moves the pieces closer together and leaves one space "
          + "empty."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/lakeward/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "terms-at-the-shelf" })
  ]);
}
