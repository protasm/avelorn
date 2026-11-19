inherit "system/place";

mapping definition() {
  return ([
    "name": "Open Roof Hall",
    "description":
        "The hall has one roof and no inner doors. Anyone fed here may ask the households to "
        + "hear a grievance. A bare [beam] holds nails for guest tokens that have never come home.",
    "details": ([
      "beam":
          "Empty nail holes retain the shapes of missing tokens. With the related work "
          + "accepted, use `trace nail` here or `lodge receipt` with the houses' bowl tally."
    ]),
    "environment": "interior",
    "exits": ([
      "south": "place/havenrode/square"
    ]),
    "services": ({ }),
    "npcs": ({ "edran-sell" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "three-shared-bowls-lodge",
        "verb": "lodge",
        "argument": "receipt",
        "text": "The registrar pins your count to the beam beside two unpaid harbor bills."
      ]),
      ([
        "tag": "a-nail-without-a-name-trace",
        "verb": "trace",
        "argument": "nail",
        "text": "You rub soot across the nail hole and lift an old token's initials from the wood."
      ])
    })
  ]);
}
