inherit "system/place";

mapping definition() {
  return ([
    "name": "Westwatch Landing",
    "description":
        "Driftwood [steps] descend between wind-bent grasses to a ferry mooring. A board lists "
        + "passages to Havenrode. The guest-house is north. The path out reaches the headland. "
        + "From there, the old signal lamp is one wilderness square east.",
    "details": ([
      "steps":
          "Salt bleaches the bottom tread while tar seals the one above it. A fresh water cask "
          + "waits on the upper tread. Once Rhea's hoop has been measured, use `fit hoop` here."
    ]),
    "environment": "settlement",
    "exits": ([
      "north": "place/westwatch/guest-house",
      "out": "place/atlas/x0113/y3336"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ "cobin-holl" }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "hoops-for-the-landing-fit",
        "verb": "fit",
        "argument": "hoop",
        "text": "You work the hoop down over the staves until the leaking seam holds."
      ])
    })
  ]);
}
