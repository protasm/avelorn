inherit "system/place";

mapping definition() {
  return ([
    "name": "Foster Lists",
    "description":
        "Each surviving [foster list] has an ordinary ink copy and a second sheet of names "
        + "hidden beneath. One of the people counted here was listed as moveless cargo in "
        + "Havenrode.",
    "details": ([
      "foster list":
          "The hidden sheet can be copied with `copy list`. Once Sella's first inquiry is "
          + "settled, `flag opt-outs` marks the clauses that let guests depart freely."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/greenhame/hearth"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "action": ([
      "tag": "copy-foster-list",
      "verb": "copy",
      "argument": "list",
      "text":
          "You make an accurate copy and return the hidden sheet before the stove's light can "
          + "catch its ink."
    ]),
    "actions": ({
      ([
        "tag": "an-empty-bunk-flag",
        "verb": "flag",
        "argument": "opt-outs",
        "text": "You mark the contract lines that give a foster guest a clear way to decline."
      ])
    })
  ]);
}
