inherit "system/place";

mapping definition() {
  return ([
    "name": "Lakeward Shelf",
    "description":
        "The path angles between a screen of dark trees and the open lake. Pack trains unload "
        + "behind a row of [barrels] so smoke from the town cannot be seen from the water. The "
        + "Cinder Bank refuge is three wilderness squares east.",
    "details": ([
      "barrels":
          "The outer barrel hides the second rescue mooring from the open water. Use `test "
          + "mooring` after inspecting the boat fittings."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2289/y3897",
      "north": "place/lakeward/hearth",
      "east": "place/lakeward/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "terms-at-the-shelf-test",
        "verb": "test",
        "argument": "mooring",
        "text":
            "The altered mooring holds both boats without stopping passengers from stepping "
            + "off."
      ])
    })
  ]);
}
