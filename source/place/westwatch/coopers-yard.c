inherit "system/place";

mapping definition() {
  return ([
    "name": "Westwatch Cooper's Yard",
    "description":
        "Barrel [hoops] hang from the branch of a salt-stunted tree. The good casks get salt "
        + "fish; battered ones go inland filled with dry tinder. An empty patch by the fence is "
        + "worn by practice footwork.",
    "details": ([
      "hoops":
          "The oak hoops have expanded in the salt air. One is bent but still sound, and a "
          + "piece of chalk lies beside it. Use `measure hoop` for the landing cask."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/westwatch/guest-house"
    ]),
    "services": ({ "market", "training", "noticeboard" }),
    "npcs": ({ "rhea-kett" }),
    "safe": 0,
    "quests": ({ "hoops-for-the-landing" }),
    "actions": ({
      ([
        "tag": "hoops-for-the-landing-measure",
        "verb": "measure",
        "argument": "hoop",
        "text": "You draw a clean chalk line across the overlap in a bent oak hoop."
      ])
    })
  ]);
}
