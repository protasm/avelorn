inherit "system/place";

mapping definition() {
  return ([
    "name": "The First Cut",
    "description":
        "Tools rust in place around the first opening. A [map] of the drainage network runs "
        + "down the wall, traced in lines of bonded metal that shiver toward the surface.",
    "details": ([
      "map":
          "The drains are marked from the vault outward, running toward ports and foundries in "
          + "all four coastal nations. Threads from distant works meet at a point inside this "
          + "wall."
    ]),
    "environment": "dungeon",
    "exits": ([
      "west": "place/deep-vault/seam-guard",
      "east": "place/deep-vault/return-shelf"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
