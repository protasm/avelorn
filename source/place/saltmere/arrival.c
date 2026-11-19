inherit "system/place";

mapping definition() {
  return ([
    "name": "Saltmere Ledge",
    "description":
        "Thin skins of sea salt line the puddles. The tender stops at a bleached wooden [pier] "
        + "with steps cut to three heights for the tide.",
    "details": ([
      "pier":
          "The bottom step serves shallow tenders even at dead low water. Knots of white cord "
          + "on the upper post tell pilots when the old steps go under."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x1097/y3844",
      "north": "place/saltmere/hearth",
      "east": "place/saltmere/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
