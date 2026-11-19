inherit "system/place";

mapping definition() {
  return ([
    "name": "Descent Hearth",
    "description":
        "A rack of loaned [crampons] dries by the door. The cabin has a low ceiling so it can "
        + "hold heat through a storm. The stove door is scarred where someone quenched a glowing "
        + "piece of metal.",
    "details": ([
      "crampons":
          "Half the straps are patched with cloth from guides who borrowed the iron and brought "
          + "it back. Avren has hung the smallest pair at a child's shoulder height."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/coalstair/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ "avren-toll" }),
    "safe": 1,
    "quests": ({ "under-an-old-wage" })
  ]);
}
