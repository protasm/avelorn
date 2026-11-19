inherit "system/place";

mapping definition() {
  return ([
    "name": "The Forge at Cinder Bank",
    "description":
        "Four practitioners keep different tools beside a common [crucible]. A woman slides "
        + "each person a cup of water and asks what help they mean to give. The low quarry door "
        + "stands open for anyone who would rather walk back out.",
    "details": ([
      "crucible":
          "The rim bears four marks made by different tools, spaced where everyone can reach. "
          + "Use `forge concordance` when Bren has given you the work or `hear practitioners` for "
          + "their accounts afterward."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/cinder-bank/reed-screen",
      "east": "place/cinder-bank/redoubt"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ "bren-rusk", "elle-hearth", "hevern-ink", "eris-reed", "maren-shore" }),
    "safe": 1,
    "action": ([
      "tag": "forge-concordance",
      "verb": "forge",
      "argument": "concordance",
      "text":
          "Each practitioner freely names the same purpose, to bring a crew back from the "
          + "buried works. A metal bead cools. Outside, a patrol bell answers a moment of "
          + "silence."
    ]),
    "quests": ({ "three-voices-at-cinder-bank" }),
    "actions": ({
      ([
        "tag": "three-voices-at-cinder-bank-hear",
        "verb": "hear",
        "argument": "practitioners",
        "text":
            "Four practitioners agree on what they did and describe the moment of cooling in "
            + "four different voices."
      ])
    })
  ]);
}
