inherit "system/place";

mapping definition() {
  return ([
    "name": "Graft Road",
    "description":
        "The [fruit trees] stand in irregular ranks where the ground holds spring warmth. A "
        + "wagon gives the right of way to a pair of carts hauling broken branches.",
    "details": ([
      "fruit trees":
          "One young tree has two tied tags, one for the grower and a blank one for its "
          + "intended household. Use `tag replacement` after checking the deed."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2842/y3920",
      "north": "place/orchard-cross/hearth",
      "east": "place/orchard-cross/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "giving-a-share-back-tag",
        "verb": "tag",
        "argument": "replacement",
        "text": "The new tree wears a plain tag recording who should receive it when spring comes."
      ])
    })
  ]);
}
