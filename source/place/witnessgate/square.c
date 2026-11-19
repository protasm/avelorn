inherit "system/place";

mapping definition() {
  return ([
    "name": "Hearing Steps",
    "description":
        "People pause here to listen as debts are acknowledged, bets are settled, and the day's "
        + "witness list is read. The Dominion's list goes first. Its witnesses stand on a higher "
        + "[stair].",
    "details": ([
      "stair":
          "Hollow steps carry a speaker's voice down into the square. The top riser bears the "
          + "ministry's seal. In heavy rain the seal fills first."
    ]),
    "environment": "settlement",
    "exits": ([
      "south": "place/witnessgate/arrival",
      "east": "place/witnessgate/market",
      "north": "place/witnessgate/hall",
      "west": "place/witnessgate/hearth",
      "up": "place/witnessgate/yard"
    ]),
    "services": ({ "noticeboard" }),
    "npcs": ({ }),
    "safe": 0,
    "quests": ({ "the-uncut-seal" })
  ]);
}
