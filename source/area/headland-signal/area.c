mapping definition() {
  return ([
    "name": "Headland Signal",
    "nation": "arvenna",
    "kind": "site",
    "anchor": ({ 114, 3336 }),
    "entrance": "place/headland-signal/lamp-walk",
    "control": "contested",
    "institution": "Pilot crews maintain a lamp to guide boats around the western tip.",
    "economy": "Signals keep Westwatch's ferry crossing open.",
    "rooms": ([
      "place/headland-signal/lamp-walk": ({ 30000004, "wilderness" }),
      "place/headland-signal/signal-dome": ({ 30000005, "interior" })
    ]),
    "routes": ([ ]),
    "trails": ([ ]),
    "landmarks": ([
      "place/atlas/x0114/y3336": ([
        "name": "Headland Signal Track",
        "description": "The lamp above the dunes has gone dark. A worn path leads to its walk."
      ])
    ]),
    "site": ([
      "area": "headland-signal",
      "level": 1,
      "summary": "Clear the pilot's lamp walk and take in the view from the signal dome."
    ])
  ]);
}
