inherit "system/place";

mapping definition() {
  return ([
    "name": "Empty Winch Road",
    "description":
        "Fresh boot prints pass a [padlock] eaten through with rust. The track north crosses a "
        + "broken axle embedded in the shale.",
    "details": ([
      "padlock":
          "The lock has rusted through, but its bow is polished where fresh gloves keep lifting "
          + "it. The prints head north with a burden dragged between them."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x2323/y3800",
      "north": "place/empty-winch/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
