inherit "system/place";

mapping definition() {
  return ([
    "name": "Paving Yard",
    "description":
        "Weights for the surveyors line a wall. Recruits learn to keep formation across wet "
        + "stones while the site foreman inspects a row of [practice gates].",
    "details": ([
      "practice gates":
          "One gate can be held by a single worker. The broad one needs help at both hinges. "
          + "Use `rehearse gates` during the paired lift drill."
    ]),
    "environment": "settlement",
    "exits": ([
      "down": "place/keepingstead/square"
    ]),
    "services": ({ "training", "noticeboard" }),
    "npcs": ({ }),
    "safe": 0,
    "quests": ({ "weir-keys" }),
    "actions": ({
      ([
        "tag": "weir-keys-rehearse",
        "verb": "rehearse",
        "argument": "gates",
        "text": "The recruits manage a paired lift without leaving the practice threshold."
      ])
    })
  ]);
}
