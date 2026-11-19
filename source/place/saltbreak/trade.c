inherit "system/place";

mapping definition() {
  return ([
    "name": "Storm Riggers",
    "description":
        "Spools of tarred rope stand on blocks above the stone floor. The rigger keeps a "
        + "[rescue line] under the counter and will cut it free before taking payment.",
    "details": ([
      "rescue line":
          "A traveler has donated a pale section of rope to repair the tarred one. Use `test "
          + "splice` before it goes out to brace the wall."
    ]),
    "environment": "settlement",
    "exits": ([
      "west": "place/saltbreak/arrival"
    ]),
    "services": ({ "market" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "the-far-wall-test",
        "verb": "test",
        "argument": "splice",
        "text":
            "The borrowed line holds through two pulls, enough to pull a loose wall back "
            + "toward its footing."
      ])
    })
  ]);
}
