inherit "system/place";

mapping definition() {
  return ([
    "name": "Stones of Refusal",
    "description":
        "A [wall] of flat stones preserves marks left by crews who declined to sail. The last "
        + "space is blank, since the bailiff judged refusal too dangerous to record.",
    "details": ([
      "wall":
          "One mark resembles the cut on the missing speaker's chair. Use `compare chair-mark` "
          + "to record which refusal is theirs."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/tide-chapel/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-chair-by-the-door-compare",
        "verb": "compare",
        "argument": "chair-mark",
        "text":
            "The chair's mark matches one on the last stone with a name. The speaker had a "
            + "recognized right to refuse."
      ])
    })
  ]);
}
