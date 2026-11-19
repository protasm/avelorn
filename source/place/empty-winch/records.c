inherit "system/place";

mapping definition() {
  return ([
    "name": "Deferred Casualties Board",
    "description":
        "A [roster] of workers marked transferred corresponds to another list hidden beneath "
        + "it. The second specifies whether a widow is to be informed of a new death.",
    "details": ([
      "roster":
          "The newest date was entered before the previous one dried. Use `inspect roster` to "
          + "record who disappeared and which household is still owed wages."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/empty-winch/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "company-wages-inspect",
        "verb": "inspect",
        "argument": "roster",
        "text": "The dates establish that one family lost two wages in the same week."
      ])
    })
  ]);
}
