inherit "system/place";

mapping definition() {
  return ([
    "name": "Double Lamp House",
    "description":
        "The original keeper's lamp stands next to one confiscated from an omitted hamlet. "
        + "Every night the keeper lights [both lamps], and two neighbors independently record "
        + "what they see.",
    "details": ([
      "both lamps":
          "The two shutters make different shapes of light even when they burn together. Use "
          + "`attest beacon` to set down the conditions of the sighting."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/beacon-holt/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ "brin-tale" }),
    "safe": 1,
    "action": ([
      "tag": "attest-the-beacon",
      "verb": "attest",
      "argument": "beacon",
      "text": "You set the time, weather, and number of lights down in front of the keeper."
    ]),
    "quests": ({ "before-bell-one" })
  ]);
}
