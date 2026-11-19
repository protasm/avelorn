inherit "system/place";

mapping definition() {
  return ([
    "name": "Moorcross Station",
    "description":
        "Rain beats a steady tick against a line of wagon tops. Drovers chalk prices on the "
        + "inside of the roadside [gate] so a patrol cannot read them without coming in.",
    "details": ([
      "gate":
          "A wet trace hangs from the hinge, its brand visible below the price chalk. Use "
          + "`inspect trace` to find who lent that team."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2321/y3483",
      "north": "place/moorcross/hearth",
      "east": "place/moorcross/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "pay-the-drover-inspect",
        "verb": "inspect",
        "argument": "trace",
        "text":
            "You follow the branded traces back to a family that was not a party to the "
            + "commission."
      ])
    })
  ]);
}
