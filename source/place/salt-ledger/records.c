inherit "system/place";

mapping definition() {
  return ([
    "name": "Guest Bond Archive",
    "description":
        "A [contract] notes that the hand over the lent grain agreed to nothing. A hand in "
        + "another ink marks which sponsor hid the apprentice's family.",
    "details": ([
      "contract":
          "Small guest tokens have been pressed flat between canceled contracts. The names are "
          + "legible under the wax. Use `copy tokens` for Hearth Row's inquiry."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/salt-ledger/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "a-nail-without-a-name-copy",
        "verb": "copy",
        "argument": "tokens",
        "text":
            "Three forfeiture entries begin with the same initials. You copy the last "
            + "signatures beside them."
      ])
    })
  ]);
}
