inherit "system/place";

mapping definition() {
  return ([
    "name": "Whitequay Customs Stalls",
    "description":
        "A [scale tray] bears a whitened wear mark where timber factors empty their cups of "
        + "lime. The crier repeats the scales' weights over the sound of the winch.",
    "details": ([
      "scale tray":
          "An ordinary timber sample sits clear of the lime in one corner. Use `weigh sample` "
          + "to check the customs calculation against a second scale."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/whitequay/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-whitequay-weighing-weigh",
        "verb": "weigh",
        "argument": "sample",
        "text":
            "The load is no heavier on the second scale, though the surcharge was assessed as "
            + "if it were."
      ])
    })
  ]);
}
