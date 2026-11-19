inherit "system/place";

mapping definition() {
  return ([
    "name": "Reedford Causeway",
    "description":
        "Dark water stands on both sides of a causeway raised with poles and bundles of reeds. "
        + "Freight carts halt where [planks] have been laid to spread their weight.",
    "details": ([
      "planks":
          "An iron pin stands out among the nails. Use `find flood-pin` while verifying the "
          + "carpenter's account of the old crossing."
    ]),
    "environment": "settlement",
    "exits": ([
      "out": "place/atlas/x2040/y4177",
      "north": "place/reedford/hearth",
      "east": "place/reedford/trade"
    ]),
    "services": ({ "transit" }),
    "npcs": ({ }),
    "safe": 0,
    "actions": ({
      ([
        "tag": "when-the-causeway-fell-find",
        "verb": "find",
        "argument": "flood-pin",
        "text": "The pin shows how high the crossing flood actually reached."
      ])
    })
  ]);
}
