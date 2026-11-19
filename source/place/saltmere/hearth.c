inherit "system/place";

mapping definition() {
  return ([
    "name": "Stranded Crew Room",
    "description":
        "Tables stand on bulkhead rings and scarcely rattle in the wind. A [carved list] names "
        + "every boat that brought a crew here and never sailed again.",
    "details": ([
      "carved list":
          "A circle beside a boat's name means nobody asked its survivors for a sponsor. Use "
          + "`count bowls` to tally the meals the crew room can still offer that way."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/saltmere/arrival"
    ]),
    "services": ({ "rest", "noticeboard" }),
    "npcs": ({ }),
    "safe": 1,
    "actions": ({
      ([
        "tag": "three-shared-bowls-count",
        "verb": "count",
        "argument": "bowls",
        "text": "You tally how many crew meals can be served without asking for a sponsor."
      ])
    }),
    "quests": ({ "salt-before-sun" })
  ]);
}
