inherit "system/place";

mapping definition() {
  return ([
    "name": "Second Wall Shelter",
    "description":
        "The seaward door is barred from the inside. The landward door stays unbarred in storm "
        + "weather. A fresh [blackboard] lists the households owed new shutters.",
    "details": ([
      "blackboard":
          "One household owes two shutters elsewhere and is owed a new one here. A second "
          + "column tracks needs the relief store can meet before debts are settled."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/saltbreak/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "the-far-wall" })
  ]);
}
