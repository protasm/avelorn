inherit "system/place";

mapping definition() {
  return ([
    "name": "Pan Bank",
    "description":
        "A woman rakes salt into the rim of a pan and wraps it in half lengths of sailcloth. A "
        + "patrol watches her [scale] because the pans owe their first measure to the Dominion.",
    "details": ([
      "scale":
          "The pan's weight is scratched beneath the levy mark. Alongside it is an older mark "
          + "for the crews' first share. Use `weigh salt` to account for both."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/saltmere/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "salt-before-sun-weigh",
        "verb": "weigh",
        "argument": "salt",
        "text":
            "You separate the traditional first share for stranded crews from the levy "
            + "demanded by the patrol."
      ])
    })
  ]);
}
