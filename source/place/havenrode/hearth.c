inherit "system/place";

mapping definition() {
  return ([
    "name": "Lower Hearth",
    "description":
        "Benches face west so guests can watch sailcloth turn gold at sunset. A shelf of "
        + "bundled [coats] is arranged by size, with no lock between the room and the street.",
    "details": ([
      "coats":
          "The sleeve pockets hold no names or house marks. A slate beneath the smallest bundle "
          + "tracks spare beds. Use `update guest-count` if you're on quay duty."
    ]),
    "environment": "interior",
    "exits": ([
      "east": "place/havenrode/square"
    ]),
    "services": ({ "rest" }),
    "npcs": ({ }),
    "safe": 1,
    "actions": ({
      ([
        "tag": "shells-beneath-the-quay-update",
        "verb": "update",
        "argument": "guest-count",
        "text": "The hearth roster reserves a bed for anyone hurt working the moving stair."
      ])
    })
  ]);
}
