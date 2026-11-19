inherit "system/place";

mapping definition() {
  return ([
    "name": "Alder Peg Stall",
    "description":
        "Eccentric handles stand in a hamper of [pegs] graduated by finger width. The tools at "
        + "the bottom are glossy from generations of repairs in water.",
    "details": ([
      "pegs":
          "The top peg is new, but the older ones show their original hinge patterns. Use "
          + "`match peg` before reading the waterline at Tile Track."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/eastweir/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-older-repair-ledger-match",
        "verb": "match",
        "argument": "peg",
        "text":
            "The notch pattern matches the old shift's spare peg, so the repair predates the "
            + "handover."
      ])
    })
  ]);
}
