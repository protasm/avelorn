inherit "system/place";

mapping definition() {
  return ([
    "name": "Walkway Shop",
    "description":
        "Lengths of marked cord hang above reed coils and carpenters' tools. A fitter reads her "
        + "[guarantee] aloud so the people waiting know what to ask for if a slat fails.",
    "details": ([
      "guarantee":
          "The fitter has cut the conditions into a wide wooden slat. Use `read guarantee` to "
          + "check what level of flood the causeway was meant to withstand."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/reedford/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "when-the-causeway-fell-read",
        "verb": "read",
        "argument": "guarantee",
        "text": "The cut guarantee explicitly covers three floods beyond the last reading."
      ])
    })
  ]);
}
