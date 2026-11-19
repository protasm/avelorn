inherit "system/place";

mapping definition() {
  return ([
    "name": "Red Run Tile Path",
    "description":
        "A [wet stripe] glows red where the bank was patched with the kiln's last batch. The "
        + "rest of the path carries the bootprints of workers called to shift early.",
    "details": ([
      "wet stripe":
          "The new tile has a deeper red than the old spillway stone. A thin seam of running "
          + "water underneath it shows where someone already needs to come back."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x3033/y3854",
      "north": "place/red-run/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
