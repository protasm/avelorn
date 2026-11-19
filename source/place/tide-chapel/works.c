inherit "system/place";

mapping definition() {
  return ([
    "name": "Press Court",
    "description":
        "A rail seats the hands waiting for transport. An unsigned [pledge] pinned to the wall "
        + "names people whose wrists carry matching red cord.",
    "details": ([
      "pledge":
          "The signatures are all in one hand, with a space below allegedly left for each "
          + "recruit. Beneath the rail, red fibers show where wrists were held against the wood."
    ]),
    "environment": "dungeon",
    "exits": ([
      "south": "place/tide-chapel/approach",
      "down": "place/tide-chapel/records"
    ]),
    "services": ({ }),
    "npcs": ({ "oath-bailiff" }),
    "safe": 0
  ]);
}
