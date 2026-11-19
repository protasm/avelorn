inherit "system/place";

mapping definition() {
  return ([
    "name": "Boiler Walk",
    "description":
        "Riveted kettle plates make a rain shelter over a row of nets. A foundry man counts out "
        + "[rivets] while his daughter sands a boat hook smooth.",
    "details": ([
      "rivets":
          "The rivet pattern changes where an old iron plate was cut to fit the second boat. "
          + "Use `check rivets` before testing the mooring."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/lakeward/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "terms-at-the-shelf-check",
        "verb": "check",
        "argument": "rivets",
        "text": "One plate needs four replacement rivets before the hook can take any load."
      ])
    })
  ]);
}
