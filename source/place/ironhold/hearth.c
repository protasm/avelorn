inherit "system/place";

mapping definition() {
  return ([
    "name": "Crew Hearth",
    "description":
        "The benches smell of wet hide, iron, and porridge. Somebody has folded spare "
        + "[blankets] in careful thirds. The notice over the hearth reads Bring Everyone Home.",
    "details": ([
      "blankets":
          "Beside the blankets a board lists crew members due back and families waiting for "
          + "food. Use `read return-list` or `set places` for the assignment you took."
    ]),
    "environment": "interior",
    "exits": ([
      "east": "place/ironhold/square"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "quests": ({ "a-light-below-the-winch" }),
    "actions": ({
      ([
        "tag": "a-light-below-the-winch-read",
        "verb": "read",
        "argument": "return-list",
        "text":
            "The old crew's status resolves to two returned and three missing. You leave one "
            + "lamp burning."
      ]),
      ([
        "tag": "the-companions-count-set",
        "verb": "set",
        "argument": "places",
        "text": "You add places for several households the foundry's current roster ignored."
      ])
    })
  ]);
}
