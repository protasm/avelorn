inherit "system/place";

mapping definition() {
  return ([
    "name": "Unpicked Shed",
    "description":
        "A [door] has been unhinged from the outside. Scrawled on it is a promise to come back "
        + "for a boiler, a brass compass, and the stocks of a small boat.",
    "details": ([
      "door":
          "The hinges were unpinned rather than broken. Elsewhere on the door are smaller "
          + "names, as if an earlier group came to help carry the goods back."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/press-yard/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
