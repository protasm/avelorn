inherit "system/place";

mapping definition() {
  return ([
    "name": "Tarcloth Market",
    "description":
        "Net weights and toweling dangle from tarred [ropes]. A cooper's apprentice argues for "
        + "cash wages in place of a place at the master's table. Farther down the row, an "
        + "occupied storefront is collecting the harbor tax.",
    "details": ([
      "ropes":
          "Tar rubs off the lowest rope onto the apprentices' shirts. Two open bills for the "
          + "same repair are clipped to it. Use `check invoices` for the wage tally."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/havenrode/square"
    ]),
    "services": ({ "market", "noticeboard" }),
    "npcs": ({ "veska-lowe" }),
    "safe": 0,
    "quests": ({ "tarbills", "shells-beneath-the-quay" }),
    "actions": ({
      ([
        "tag": "tarbills-check",
        "verb": "check",
        "argument": "invoices",
        "text": "The same bar of tar appears on two accounts. You mark the duplicated charge."
      ])
    })
  ]);
}
